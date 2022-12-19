#include "stdafx.h"
#include "czmq.h"
#include "VirtualDevice.h"

VirtualDevice::VirtualDevice()
	:actor(nullptr),
	pipeSock(nullptr),
	zloop(nullptr),
	server(nullptr),
	connected(false),
	heartbeatTimerID(0),
	m_cmdId(0),
	m_cmdIndex(0),
	m_connectStatus(UNCONNECTED),
	m_receiveNum(1),
	m_channelType("ZK"),
	m_quitFlag(false)
{
}

VirtualDevice::~VirtualDevice()
{

}

void VirtualDevice::Start(const string& addr)
{
	this->m_addr = addr;
	actor = zactor_new(LoopFuncWrap, this);
}

void VirtualDevice::LoopFuncWrap(zsock_t *pipe, void *args)
{
	VirtualDevice* self = (VirtualDevice*)args;
	self->LoopFunc(pipe);
}

void VirtualDevice::LoopFunc(zsock_t *pipe)
{
	zsock_signal(pipe, 0);
	pipeSock = pipe;
	//初始化设备端口监听
	server = zsock_new(ZMQ_STREAM, m_addr.c_str());
	zloop = zloop_new();
	int rc = zloop_reader(zloop, server, ServerEventHandler, this);
	zloop_start(zloop);
	//循环结束后清理
	zloop_destroy(&zloop);
	zsock_destroy(&server);

	{
		std::lock_guard<std::mutex> lg(m_readyMutex);///锁住readymutex
		m_readyFlag = true;///更新readyFlag
	}///解锁
	m_readyCondVar.notify_one();///通知动作 唤醒等待中的线程
}

int VirtualDevice::ServerEventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	VirtualDevice* self = (VirtualDevice*)arg;
	return self->OnServerEvent();
}

int VirtualDevice::OnServerEvent()
{
	zmsg_t* request = zmsg_recv(server);
	if (request == nullptr)
		return -1;
	assert(zmsg_size(request)== 2);
	zframe_t* id_frame = zmsg_first(request);
	int idSize = zframe_size(id_frame);
	zframe_t* data_frame = zmsg_next(request);
	int frameSize = zframe_size(data_frame);
	if (frameSize)
	{
		if (connected)
		{
			if (m_channelType == "ZK")
			{
				zsys_debug("-------------------第%d次接收", m_receiveNum);
				ParsreData(data_frame, &m_cmdId, &m_cmdIndex);
				SendOneResult();
				m_receiveNum++;
			}
			else
			{
				SendReply();
			}
		}
		else
		{
			m_channelType = (char*)zframe_data(data_frame);
			zsys_debug("Open Channel:%s", (char*)zframe_data(data_frame));
			connected = true;
		}
	}
	else
	{
		if (m_clientId.empty())
		{
			m_clientId.resize(idSize);
			memcpy(m_clientId.data(), zframe_data(id_frame), idSize);
			zsys_debug("Connected...");
		}
		else
		{
			m_clientId.clear();
			zsys_debug("DisConnected...");
			connected = false;
		}
	}
	zmsg_destroy(&request);
	//2nd frame
	//if null
	//else
	//1. 解析
}

void VirtualDevice::ParsreData(zframe_t* data_frame, uint32_t* commandid, uint32_t* indexcmd)
{
	assert(zframe_size(data_frame) != 0);
	recieveDataFrame dataReceive;
	dataReceive.header.syncword = ((recieveDataFrame*)zframe_data(data_frame))->header.syncword;
	dataReceive.header.byteLength = ((recieveDataFrame*)zframe_data(data_frame))->header.byteLength;
	dataReceive.commandID = ((recieveDataFrame*)zframe_data(data_frame))->commandID;
	dataReceive.counter = ((recieveDataFrame*)zframe_data(data_frame))->counter;
	for (int i = 0; i < 27; i++)
	{
		dataReceive.dataBlock[i] = ((recieveDataFrame*)zframe_data(data_frame))->dataBlock[i];
	}
	dataReceive.checkSum = ((recieveDataFrame*)zframe_data(data_frame))->checkSum;
	uint32_t commandID = dataReceive.commandID;
	*commandid = commandID;
	*indexcmd = dataReceive.counter;
	PrintResult(dataReceive);
}

void VirtualDevice::PrintResult(recieveDataFrame& dataReceive)
{
	uint32_t commandID = dataReceive.commandID;
	if (SELF_DETECT_ID == commandID)
	{
		printf("接收自检指令,命令ID号:%0x\n", commandID);
	}
	else if (START_STOP_ID == commandID)
	{
		printf("接收启动/停止指令,命令ID号:%0x\n", commandID);
		printf("启动/停止:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (LOCK_TRACK_ID == commandID)
	{
		printf("接收锁定跟踪指令,命令ID号:%0x\n", commandID);
		printf("搜索起始频率:%0x\n", dataReceive.dataBlock[2]);
		printf("目标信号类型:%0x\n", dataReceive.dataBlock[10]);
	}
	else if (RESET_ID == commandID)
	{
		printf("接收复位指令,命令ID号:%0x\n", commandID);
	}
	else if (PARA_CONFIG_ID == commandID)
	{
		printf("接收参数配置指令,命令ID号:%0x\n", commandID);
		printf("搜索起始频率:%0x\n", dataReceive.dataBlock[2]);
		printf("目标信号类型:%0x\n", dataReceive.dataBlock[10]);
	}
	else if (GPS_MODUL_CONFIG_ID == commandID)
	{
		printf("接收GPS模块配置指令,命令ID号:%0x\n", commandID);
	}
	else if (COMPASS_CALIBRATION_ID == commandID)
	{
		printf("接收电子罗盘校准指令,命令ID号:%0x\n", commandID);
	}
	else if (IP_CONFIG_ID == commandID)
	{
		printf("接收监测节点IP配置指令,命令ID号:%0x\n", commandID);
		printf("IP地址:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (LOW_POWER_CONSUMPTION_ID == commandID)
	{
		printf("接收低功耗指令,命令ID号:%0x\n", commandID);
		printf("进入/退出低功耗:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (WAVE_GATE_SWITCH_ID == commandID)
	{
		printf("接收波门开关指令,命令ID号:%0x\n", commandID);
		printf("波门开关:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (AGC_CONTROL_ID == commandID)
	{
		printf("接收AGC控制指令,命令ID号:%0x\n", commandID);
		printf("开闭环控制:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (DETECT_THRESHOLD_ADJUST_ID == commandID)
	{
		printf("接收检测门限调整指令,命令ID号:%0x\n", commandID);
		printf("检测门限值:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (SOFTWARE_UPDATE_ID == commandID)
	{
		printf("接收软件更新指令,命令ID号:%0x\n", commandID);
		printf("开始/结束:%0x\n", dataReceive.dataBlock[0]);
		printf("软件类型:%0x\n", dataReceive.dataBlock[1]);
	}
	else if (STANDARD_CALIBRATION_ID == commandID)
	{
		printf("接收标校指令,命令ID号:%0x\n", commandID);
		printf("频率值:%0x\n", dataReceive.dataBlock[0]);
	}
	printf("命令计数:%d\n", dataReceive.counter);
	printf("校验和:%d\n", dataReceive.checkSum);
}


void VirtualDevice::SendOneResult()
{
	if (m_cmdId == START_STOP_ID || m_cmdId == LOCK_TRACK_ID)
	{
		ReplyResult(TOTAL_BYTE_LENGTH_256,1);
	}
	else
	{
		ReplyResult(TOTAL_BYTE_LENGTH_128, 1);
	}
}

void VirtualDevice::ReplyResult(uint32_t totalByteNum, uint32_t response)
{
	assert(!m_clientId.empty());
	int totalcount = totalByteNum / sizeof(uint32_t);//回传32个uint32_t类型数据
	int index = 0;
	while (index < totalcount)
	{
		zmsg_t* reply = zmsg_new();
		zmsg_addmem(reply, m_clientId.data(), m_clientId.size());///把peer id打包进响应
		uint32_t data = 0X8D5F2C6B;
		if (index % totalcount == 0)
		{
			data = SYNC_WORD_ZK;
		}
		if (index % totalcount == 1)
		{
			data = totalByteNum - NO_BYTE_LENGTH;
		}
		if (index % totalcount == 2)
		{
			data = m_cmdId;
		}
		if (index % totalcount == 3)
		{
			data = response;
		}
		if (index % totalcount == 4)
		{
			data = m_cmdIndex;
		}
		if (index % totalcount == 5)
		{
			data = 0XCE41CE41;
		}
		zframe_t *dadaframe = zframe_new(&data, sizeof(data));
		zmsg_append(reply, &dadaframe);					///把回送的数据打包进响应
		int sendFlag = zmsg_send(&reply, server);		    ///发送响应
		index++;
	}
}


void VirtualDevice::SendReply()
{
	while (true)
	{
		ReplyData(0x10000008, TOTAL_BYTE_LENGTH_YC);
	}
}

void VirtualDevice::ReplyData(uint32_t commandid, uint32_t totalbytenum)
{
	assert(!m_clientId.empty());
	int totalcount = totalbytenum / sizeof(uint32_t);//回传44个uint32_t类型数据
	int index = 0;
	while (index < totalcount)
	{
		zmsg_t* reply = zmsg_new();
		zmsg_addmem(reply, m_clientId.data(), m_clientId.size());///把peer id打包进响应
		uint32_t data = 0X8D5F2C6B;
		if (index % totalcount == 0)
		{
			data = SYNC_WORD_YC;
		}
		if (index % totalcount == 1)
		{
			data = TOTAL_BYTE_LENGTH_YC - NO_BYTE_LENGTH;
		}
		if (index % totalcount == 2)
		{
			data = commandid;
		}
		if (index % totalcount == 3)
		{
			data = 0XCE41CE41;
		}
		zframe_t *dadaframe = zframe_new(&data, sizeof(data));
		zmsg_append(reply, &dadaframe);					///把回送的数据打包进响应
		int sendFlag = zmsg_send(&reply, server);		    ///发送响应
		index++;
	}
}

void VirtualDevice::Wait()
{
	std::unique_lock<std::mutex> ul(m_readyMutex);///锁住readymutex
	while (!m_readyFlag)///检查条件
	{
		m_readyCondVar.wait(ul); ///等待被通知 阻塞
	} ///解锁
}




//void VirtualDevice::OnReceive(zmsg_t* msg)
//{
//	string channeltype;
//	ConnectStatus m_status = UNCONNECTED;
//	while (true)
//	{
//		zmsg_t* request = zmsg_recv(server);		             ///阻塞式获取消息
//		auto msg_len = zmsg_size(request);
//		if (request != nullptr)
//		{
//			assert(zmsg_size(request) == 2);	            ///消息尺寸固定为2个frame
//			zframe_t* id_frame = zmsg_pop(request);         ///第一个frame为peer id
//			size_t size = zframe_size(id_frame);
//			vector<byte> id(size);
//			memcpy(id.data(), zframe_data(id_frame), size); ///得到peer id
//			zframe_t* body = zmsg_pop(request);
//			int frameSize = zframe_size(body);
//			if (frameSize)
//			{
//				if (m_status == CONNECTED)
//				{
//					printf("----------------------------------------------第%d次接收:\n", num);
//					uint32_t commandid;
//					uint32_t indexcmd;
//					NodeRecieve(body, &commandid, &indexcmd, 1);
//					//while (true)
//					//{
//					NodeReply(sock, id, commandid, indexcmd, channeltype);
//					//}
//					num++;
//				}
//				else
//				{
//					m_status = CONNECTED;
//					channeltype = (char*)zframe_data(body);
//					printf("开启通道:%s\n", (char*)zframe_data(body));
//				}
//			}
//			else
//			{
//				if (m_status == UNCONNECTED)
//				{
//					printf("%s\n", "客户端已连接...");
//				}
//				else
//				{
//					printf("%s\n", "客户端断开连接...\n");
//					m_status = UNCONNECTED;
//				}
//			}
//			zframe_destroy(&id_frame);
//			zframe_destroy(&body);
//			zmsg_destroy(&request);
//		}
//	}
//}
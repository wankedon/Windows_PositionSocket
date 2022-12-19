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
	//��ʼ���豸�˿ڼ���
	server = zsock_new(ZMQ_STREAM, m_addr.c_str());
	zloop = zloop_new();
	int rc = zloop_reader(zloop, server, ServerEventHandler, this);
	zloop_start(zloop);
	//ѭ������������
	zloop_destroy(&zloop);
	zsock_destroy(&server);

	{
		std::lock_guard<std::mutex> lg(m_readyMutex);///��סreadymutex
		m_readyFlag = true;///����readyFlag
	}///����
	m_readyCondVar.notify_one();///֪ͨ���� ���ѵȴ��е��߳�
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
				zsys_debug("-------------------��%d�ν���", m_receiveNum);
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
	//1. ����
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
		printf("�����Լ�ָ��,����ID��:%0x\n", commandID);
	}
	else if (START_STOP_ID == commandID)
	{
		printf("��������/ָֹͣ��,����ID��:%0x\n", commandID);
		printf("����/ֹͣ:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (LOCK_TRACK_ID == commandID)
	{
		printf("������������ָ��,����ID��:%0x\n", commandID);
		printf("������ʼƵ��:%0x\n", dataReceive.dataBlock[2]);
		printf("Ŀ���ź�����:%0x\n", dataReceive.dataBlock[10]);
	}
	else if (RESET_ID == commandID)
	{
		printf("���ո�λָ��,����ID��:%0x\n", commandID);
	}
	else if (PARA_CONFIG_ID == commandID)
	{
		printf("���ղ�������ָ��,����ID��:%0x\n", commandID);
		printf("������ʼƵ��:%0x\n", dataReceive.dataBlock[2]);
		printf("Ŀ���ź�����:%0x\n", dataReceive.dataBlock[10]);
	}
	else if (GPS_MODUL_CONFIG_ID == commandID)
	{
		printf("����GPSģ������ָ��,����ID��:%0x\n", commandID);
	}
	else if (COMPASS_CALIBRATION_ID == commandID)
	{
		printf("���յ�������У׼ָ��,����ID��:%0x\n", commandID);
	}
	else if (IP_CONFIG_ID == commandID)
	{
		printf("���ռ��ڵ�IP����ָ��,����ID��:%0x\n", commandID);
		printf("IP��ַ:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (LOW_POWER_CONSUMPTION_ID == commandID)
	{
		printf("���յ͹���ָ��,����ID��:%0x\n", commandID);
		printf("����/�˳��͹���:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (WAVE_GATE_SWITCH_ID == commandID)
	{
		printf("���ղ��ſ���ָ��,����ID��:%0x\n", commandID);
		printf("���ſ���:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (AGC_CONTROL_ID == commandID)
	{
		printf("����AGC����ָ��,����ID��:%0x\n", commandID);
		printf("���ջ�����:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (DETECT_THRESHOLD_ADJUST_ID == commandID)
	{
		printf("���ռ�����޵���ָ��,����ID��:%0x\n", commandID);
		printf("�������ֵ:%0x\n", dataReceive.dataBlock[0]);
	}
	else if (SOFTWARE_UPDATE_ID == commandID)
	{
		printf("�����������ָ��,����ID��:%0x\n", commandID);
		printf("��ʼ/����:%0x\n", dataReceive.dataBlock[0]);
		printf("�������:%0x\n", dataReceive.dataBlock[1]);
	}
	else if (STANDARD_CALIBRATION_ID == commandID)
	{
		printf("���ձ�Уָ��,����ID��:%0x\n", commandID);
		printf("Ƶ��ֵ:%0x\n", dataReceive.dataBlock[0]);
	}
	printf("�������:%d\n", dataReceive.counter);
	printf("У���:%d\n", dataReceive.checkSum);
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
	int totalcount = totalByteNum / sizeof(uint32_t);//�ش�32��uint32_t��������
	int index = 0;
	while (index < totalcount)
	{
		zmsg_t* reply = zmsg_new();
		zmsg_addmem(reply, m_clientId.data(), m_clientId.size());///��peer id�������Ӧ
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
		zmsg_append(reply, &dadaframe);					///�ѻ��͵����ݴ������Ӧ
		int sendFlag = zmsg_send(&reply, server);		    ///������Ӧ
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
	int totalcount = totalbytenum / sizeof(uint32_t);//�ش�44��uint32_t��������
	int index = 0;
	while (index < totalcount)
	{
		zmsg_t* reply = zmsg_new();
		zmsg_addmem(reply, m_clientId.data(), m_clientId.size());///��peer id�������Ӧ
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
		zmsg_append(reply, &dadaframe);					///�ѻ��͵����ݴ������Ӧ
		int sendFlag = zmsg_send(&reply, server);		    ///������Ӧ
		index++;
	}
}

void VirtualDevice::Wait()
{
	std::unique_lock<std::mutex> ul(m_readyMutex);///��סreadymutex
	while (!m_readyFlag)///�������
	{
		m_readyCondVar.wait(ul); ///�ȴ���֪ͨ ����
	} ///����
}




//void VirtualDevice::OnReceive(zmsg_t* msg)
//{
//	string channeltype;
//	ConnectStatus m_status = UNCONNECTED;
//	while (true)
//	{
//		zmsg_t* request = zmsg_recv(server);		             ///����ʽ��ȡ��Ϣ
//		auto msg_len = zmsg_size(request);
//		if (request != nullptr)
//		{
//			assert(zmsg_size(request) == 2);	            ///��Ϣ�ߴ�̶�Ϊ2��frame
//			zframe_t* id_frame = zmsg_pop(request);         ///��һ��frameΪpeer id
//			size_t size = zframe_size(id_frame);
//			vector<byte> id(size);
//			memcpy(id.data(), zframe_data(id_frame), size); ///�õ�peer id
//			zframe_t* body = zmsg_pop(request);
//			int frameSize = zframe_size(body);
//			if (frameSize)
//			{
//				if (m_status == CONNECTED)
//				{
//					printf("----------------------------------------------��%d�ν���:\n", num);
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
//					printf("����ͨ��:%s\n", (char*)zframe_data(body));
//				}
//			}
//			else
//			{
//				if (m_status == UNCONNECTED)
//				{
//					printf("%s\n", "�ͻ���������...");
//				}
//				else
//				{
//					printf("%s\n", "�ͻ��˶Ͽ�����...\n");
//					m_status = UNCONNECTED;
//				}
//			}
//			zframe_destroy(&id_frame);
//			zframe_destroy(&body);
//			zmsg_destroy(&request);
//		}
//	}
//}
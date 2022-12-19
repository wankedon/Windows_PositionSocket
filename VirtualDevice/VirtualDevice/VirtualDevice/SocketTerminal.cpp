// SocketTerminal.cpp : 定义控制台应用程序的入口点。
//
#include <random>
#include "precompile.h"
#include "VirtualDevice.h"


/**
* @brief 检测节点入口函数
*/
//int main()
//{
//	zsock_t* sock = zsock_new(ZMQ_STREAM);		             ///创建原生socket
//	zsock_bind(sock, "tcp://127.0.0.1:5555");	             ///监听本地5555端口
//	int num = 1;
//	string channeltype;
//	ConnectStatus m_status = UNCONNECTED;
//	while (true)
//	{
//		zmsg_t* request = zmsg_recv(sock);		             ///阻塞式获取消息
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
//				 	uint32_t commandid;
//					uint32_t indexcmd;
//					NodeRecieve(body, &commandid, &indexcmd, 1);
//					//while (true)
//					//{
//						NodeReply(sock, id, commandid, indexcmd, channeltype);
//					//}
//					num++;
//				}
//				else
//				{	m_status = CONNECTED;
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

/**
* @brief 接收控制端发来的数据
* @param[in] body socket接收的数据
* @param[out] id 命令ID号
* @param[out] indexcmd 接收的次数
* @return void
*/
//void NodeRecieve(zframe_t* body, uint32_t* commandid, uint32_t* indexcmd, int printSwitch)
//{
//	recieveDataFrame dataReceive;
//	dataReceive.header.syncword = ((recieveDataFrame*)zframe_data(body))->header.syncword;
//	dataReceive.header.byteLength = ((recieveDataFrame*)zframe_data(body))->header.byteLength;
//	dataReceive.commandID = ((recieveDataFrame*)zframe_data(body))->commandID;
//	dataReceive.counter = ((recieveDataFrame*)zframe_data(body))->counter;
//	for (int i = 0; i < 27; i++)
//	{
//		dataReceive.dataBlock[i] = ((recieveDataFrame*)zframe_data(body))->dataBlock[i];
//	}
//	dataReceive.checkSum = ((recieveDataFrame*)zframe_data(body))->checkSum;
//	uint32_t commandID = dataReceive.commandID;
//	*commandid = commandID;
//	*indexcmd = dataReceive.counter;
//	if (printSwitch == 1)
//	{
//		NodeRecieveParsre(dataReceive);
//		printf("命令计数:%d\n", dataReceive.counter);
//		printf("校验和:%d\n", dataReceive.checkSum);
//	}
//}

/**
* @brief 解析接收的数据
* @param[in] commandID 命令ID号
* @return void
*/

//void NodeRecieveParsre(recieveDataFrame& dataReceive)
//{
//	uint32_t commandID = dataReceive.commandID;
//	if (SELF_DETECT_ID == commandID)
//	{
//		printf("接收自检指令,命令ID号:%0x\n", commandID);
//	}
//	else if (START_STOP_ID == commandID)
//	{
//		printf("接收启动/停止指令,命令ID号:%0x\n", commandID);
//		printf("启动/停止:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (LOCK_TRACK_ID == commandID)
//	{
//		printf("接收锁定跟踪指令,命令ID号:%0x\n", commandID);
//		printf("搜索起始频率:%0x\n", dataReceive.dataBlock[2]);
//		printf("目标信号类型:%0x\n", dataReceive.dataBlock[10]);
//	}
//	else if (RESET_ID == commandID)
//	{
//		printf("接收复位指令,命令ID号:%0x\n", commandID);
//	}
//	else if (PARA_CONFIG_ID == commandID)
//	{
//		printf("接收参数配置指令,命令ID号:%0x\n", commandID);
//		printf("搜索起始频率:%0x\n", dataReceive.dataBlock[2]);
//		printf("目标信号类型:%0x\n", dataReceive.dataBlock[10]);
//	}
//	else if (GPS_MODUL_CONFIG_ID == commandID)
//	{
//		printf("接收GPS模块配置指令,命令ID号:%0x\n", commandID);
//	}
//	else if (COMPASS_CALIBRATION_ID == commandID)
//	{
//		printf("接收电子罗盘校准指令,命令ID号:%0x\n", commandID);
//	}
//	else if (IP_CONFIG_ID == commandID)
//	{
//		printf("接收监测节点IP配置指令,命令ID号:%0x\n", commandID);
//		printf("IP地址:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (LOW_POWER_CONSUMPTION_ID == commandID)
//	{
//		printf("接收低功耗指令,命令ID号:%0x\n", commandID);
//		printf("进入/退出低功耗:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (WAVE_GATE_SWITCH_ID == commandID)
//	{
//		printf("接收波门开关指令,命令ID号:%0x\n", commandID);
//		printf("波门开关:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (AGC_CONTROL_ID == commandID)
//	{
//		printf("接收AGC控制指令,命令ID号:%0x\n", commandID);
//		printf("开闭环控制:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (DETECT_THRESHOLD_ADJUST_ID == commandID)
//	{
//		printf("接收检测门限调整指令,命令ID号:%0x\n", commandID);
//		printf("检测门限值:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (SOFTWARE_UPDATE_ID == commandID)
//	{
//		printf("接收软件更新指令,命令ID号:%0x\n", commandID);
//		printf("开始/结束:%0x\n", dataReceive.dataBlock[0]);
//		printf("软件类型:%0x\n", dataReceive.dataBlock[1]);
//	}
//	else if (STANDARD_CALIBRATION_ID == commandID)
//	{
//		printf("接收标校指令,命令ID号:%0x\n", commandID);
//		printf("频率值:%0x\n", dataReceive.dataBlock[0]);
//	}
//}



//void NodeReply(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t indexcmd, string channeltype)
//{
//	if (channeltype == "ZK")
//	{
//		if (commandid == START_STOP_ID || commandid == LOCK_TRACK_ID)
//		{
//			NodeReplyResult(sock, id, commandid, TOTAL_BYTE_LENGTH_256, 1, indexcmd);
//		}
//		else
//		{
//			NodeReplyResult(sock, id, commandid, TOTAL_BYTE_LENGTH_128, 1, indexcmd);
//		}
//	}
//	else
//	{
//		NodeReplyRemoteData(sock, id, 0x10000008, TOTAL_BYTE_LENGTH_YC);
//	}
//}

/**
* @brief 回传控制结果
* @param[in] sock 套接字
* @param[in] id 套接字对应的id
* @param[in] commandid 命令ID号
* @param[in] response 指令响应标志
* @param[in] indexcmd 指令响应次数
* @return void
*/
//void NodeReplyResult(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum,uint32_t response, uint32_t indexcmd)
//{
//	int totalcount = totalbytenum / sizeof(uint32_t);//回传32个uint32_t类型数据
//	int index = 0;
//	while (index < totalcount)
//	{
//		zmsg_t* reply = zmsg_new();
//		zmsg_addmem(reply, id.data(), id.size());///把peer id打包进响应
//		uint32_t data = 0X8D5F2C6B;
//		if (index % totalcount == 0)
//		{
//			data = SYNC_WORD_ZK;
//		}
//		if (index % totalcount == 1)
//		{
//			data = totalbytenum - NO_BYTE_LENGTH;
//		}
//		if (index % totalcount == 2)
//		{
//			data = commandid;
//		}
//		if (index % totalcount == 3)
//		{
//			data = response;
//		}
//		if (index % totalcount == 4)
//		{
//			data = indexcmd;
//		}
//		if (index % totalcount == 5)
//		{
//			data = 0XCE41CE41;
//		}
//		zframe_t *dadaframe = zframe_new(&data, sizeof(data));
//		zmsg_append(reply, &dadaframe);					///把回送的数据打包进响应
//		int sendFlag = zmsg_send(&reply, sock);		    ///发送响应
//		index++;
//	}
//}

/**
* @brief 回传控制结果(随机数)
* @param[in] sock 套接字
* @param[in] id 套接字对应的id
* @param[in] commandid 命令ID号
* @param[in] response 指令响应标志
* @param[in] indexcmd 指令响应次数
* @return void
*/

/*
void NodeReplyResult(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum, uint32_t response, uint32_t indexcmd)
{
	int totalcount = totalbytenum / sizeof(uint32_t);//回传32个uint32_t类型数据
	std::random_device rd;
	std::mt19937 gen(rd());	//随机生成器引擎
	std::normal_distribution<double> degreeDis(180, 1);	//角度服从u：180、σ：1的正态分布
	std::uniform_int_distribution<> targetNumDis(1, 3);	//目标数量服从1-3的平均分布
	for (int sweepIdx = 0; sweepIdx < 3; sweepIdx++)	//模拟扫描3次
	{
		for (int segIdx = 0; segIdx < 2; segIdx++)	//模拟一次扫描2个频段
		{
			int targetNum = targetNumDis(gen);	//模拟每个频段随机产生targetNum个目标
			for (int targetIdx = 0; targetIdx < targetNum; targetIdx++)
			{
				int index = 0;
				while (index < totalcount)
				{
					zmsg_t* reply = zmsg_new();
					zmsg_addmem(reply, id.data(), id.size());///把peer id打包进响应
					uint32_t data = 0X8D5F2C6B;
					if (index % totalcount == 0)
					{
						data = SYNC_WORD_ZK;
					}
					if (index % totalcount == 1)
					{
						data = totalbytenum - NO_BYTE_LENGTH;
					}
					if (index % totalcount == 2)
					{
						data = commandid;
					}
					if (index % totalcount == 3)
					{
						data = response;
					}
					if (index % totalcount == 4)
					{
						data = indexcmd;
					}
					if (index % totalcount == 5)
					{
						data = 0XCE41CE41;
					}
					if (index % totalcount == 14)
					{
						data = degreeDis(gen);//***
					}
					if (index % totalcount == 15)
					{
						data = degreeDis(gen);
					}
					if (index % totalcount == 16)
					{
						data = sweepIdx;
					}
					if (index % totalcount == 17)
					{
						data = segIdx;
					}

					zframe_t *dadaframe = zframe_new(&data, sizeof(data));
					zmsg_append(reply, &dadaframe);					///把回送的数据打包进响应
					int sendFlag = zmsg_send(&reply, sock);		    ///发送响应
					index++;
				}
			}
		}
	}
}
*/

/**
* @brief 回传遥测数据
* @param[in] sock 套接字
* @param[in] id 套接字对应的id
* @param[in] commandid 命令ID号
* @return void
*/
//void NodeReplyRemoteData(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum)
//{
//	int totalcount = totalbytenum / sizeof(uint32_t);//回传44个uint32_t类型数据
//	int index = 0;
//	while (index < totalcount)
//	{
//		zmsg_t* reply = zmsg_new();
//		zmsg_addmem(reply, id.data(), id.size());///把peer id打包进响应
//		uint32_t data = 0X8D5F2C6B;
//		if (index % totalcount == 0)
//		{
//			data = SYNC_WORD_YC;
//		}
//		if (index % totalcount == 1)
//		{
//			data = TOTAL_BYTE_LENGTH_YC - NO_BYTE_LENGTH;
//		}
//		if (index % totalcount == 2)
//		{
//			data = commandid;
//		}
//		if (index % totalcount == 3)
//		{
//			data = 0XCE41CE41;
//		}
//		zframe_t *dadaframe = zframe_new(&data, sizeof(data));
//		zmsg_append(reply, &dadaframe);					///把回送的数据打包进响应
//		int sendFlag = zmsg_send(&reply, sock);		    ///发送响应
//		index++;
//	}
//}



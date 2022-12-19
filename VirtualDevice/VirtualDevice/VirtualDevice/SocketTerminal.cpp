// SocketTerminal.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <random>
#include "precompile.h"
#include "VirtualDevice.h"


/**
* @brief ���ڵ���ں���
*/
//int main()
//{
//	zsock_t* sock = zsock_new(ZMQ_STREAM);		             ///����ԭ��socket
//	zsock_bind(sock, "tcp://127.0.0.1:5555");	             ///��������5555�˿�
//	int num = 1;
//	string channeltype;
//	ConnectStatus m_status = UNCONNECTED;
//	while (true)
//	{
//		zmsg_t* request = zmsg_recv(sock);		             ///����ʽ��ȡ��Ϣ
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

/**
* @brief ���տ��ƶ˷���������
* @param[in] body socket���յ�����
* @param[out] id ����ID��
* @param[out] indexcmd ���յĴ���
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
//		printf("�������:%d\n", dataReceive.counter);
//		printf("У���:%d\n", dataReceive.checkSum);
//	}
//}

/**
* @brief �������յ�����
* @param[in] commandID ����ID��
* @return void
*/

//void NodeRecieveParsre(recieveDataFrame& dataReceive)
//{
//	uint32_t commandID = dataReceive.commandID;
//	if (SELF_DETECT_ID == commandID)
//	{
//		printf("�����Լ�ָ��,����ID��:%0x\n", commandID);
//	}
//	else if (START_STOP_ID == commandID)
//	{
//		printf("��������/ָֹͣ��,����ID��:%0x\n", commandID);
//		printf("����/ֹͣ:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (LOCK_TRACK_ID == commandID)
//	{
//		printf("������������ָ��,����ID��:%0x\n", commandID);
//		printf("������ʼƵ��:%0x\n", dataReceive.dataBlock[2]);
//		printf("Ŀ���ź�����:%0x\n", dataReceive.dataBlock[10]);
//	}
//	else if (RESET_ID == commandID)
//	{
//		printf("���ո�λָ��,����ID��:%0x\n", commandID);
//	}
//	else if (PARA_CONFIG_ID == commandID)
//	{
//		printf("���ղ�������ָ��,����ID��:%0x\n", commandID);
//		printf("������ʼƵ��:%0x\n", dataReceive.dataBlock[2]);
//		printf("Ŀ���ź�����:%0x\n", dataReceive.dataBlock[10]);
//	}
//	else if (GPS_MODUL_CONFIG_ID == commandID)
//	{
//		printf("����GPSģ������ָ��,����ID��:%0x\n", commandID);
//	}
//	else if (COMPASS_CALIBRATION_ID == commandID)
//	{
//		printf("���յ�������У׼ָ��,����ID��:%0x\n", commandID);
//	}
//	else if (IP_CONFIG_ID == commandID)
//	{
//		printf("���ռ��ڵ�IP����ָ��,����ID��:%0x\n", commandID);
//		printf("IP��ַ:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (LOW_POWER_CONSUMPTION_ID == commandID)
//	{
//		printf("���յ͹���ָ��,����ID��:%0x\n", commandID);
//		printf("����/�˳��͹���:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (WAVE_GATE_SWITCH_ID == commandID)
//	{
//		printf("���ղ��ſ���ָ��,����ID��:%0x\n", commandID);
//		printf("���ſ���:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (AGC_CONTROL_ID == commandID)
//	{
//		printf("����AGC����ָ��,����ID��:%0x\n", commandID);
//		printf("���ջ�����:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (DETECT_THRESHOLD_ADJUST_ID == commandID)
//	{
//		printf("���ռ�����޵���ָ��,����ID��:%0x\n", commandID);
//		printf("�������ֵ:%0x\n", dataReceive.dataBlock[0]);
//	}
//	else if (SOFTWARE_UPDATE_ID == commandID)
//	{
//		printf("�����������ָ��,����ID��:%0x\n", commandID);
//		printf("��ʼ/����:%0x\n", dataReceive.dataBlock[0]);
//		printf("�������:%0x\n", dataReceive.dataBlock[1]);
//	}
//	else if (STANDARD_CALIBRATION_ID == commandID)
//	{
//		printf("���ձ�Уָ��,����ID��:%0x\n", commandID);
//		printf("Ƶ��ֵ:%0x\n", dataReceive.dataBlock[0]);
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
* @brief �ش����ƽ��
* @param[in] sock �׽���
* @param[in] id �׽��ֶ�Ӧ��id
* @param[in] commandid ����ID��
* @param[in] response ָ����Ӧ��־
* @param[in] indexcmd ָ����Ӧ����
* @return void
*/
//void NodeReplyResult(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum,uint32_t response, uint32_t indexcmd)
//{
//	int totalcount = totalbytenum / sizeof(uint32_t);//�ش�32��uint32_t��������
//	int index = 0;
//	while (index < totalcount)
//	{
//		zmsg_t* reply = zmsg_new();
//		zmsg_addmem(reply, id.data(), id.size());///��peer id�������Ӧ
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
//		zmsg_append(reply, &dadaframe);					///�ѻ��͵����ݴ������Ӧ
//		int sendFlag = zmsg_send(&reply, sock);		    ///������Ӧ
//		index++;
//	}
//}

/**
* @brief �ش����ƽ��(�����)
* @param[in] sock �׽���
* @param[in] id �׽��ֶ�Ӧ��id
* @param[in] commandid ����ID��
* @param[in] response ָ����Ӧ��־
* @param[in] indexcmd ָ����Ӧ����
* @return void
*/

/*
void NodeReplyResult(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum, uint32_t response, uint32_t indexcmd)
{
	int totalcount = totalbytenum / sizeof(uint32_t);//�ش�32��uint32_t��������
	std::random_device rd;
	std::mt19937 gen(rd());	//�������������
	std::normal_distribution<double> degreeDis(180, 1);	//�Ƕȷ���u��180���ң�1����̬�ֲ�
	std::uniform_int_distribution<> targetNumDis(1, 3);	//Ŀ����������1-3��ƽ���ֲ�
	for (int sweepIdx = 0; sweepIdx < 3; sweepIdx++)	//ģ��ɨ��3��
	{
		for (int segIdx = 0; segIdx < 2; segIdx++)	//ģ��һ��ɨ��2��Ƶ��
		{
			int targetNum = targetNumDis(gen);	//ģ��ÿ��Ƶ���������targetNum��Ŀ��
			for (int targetIdx = 0; targetIdx < targetNum; targetIdx++)
			{
				int index = 0;
				while (index < totalcount)
				{
					zmsg_t* reply = zmsg_new();
					zmsg_addmem(reply, id.data(), id.size());///��peer id�������Ӧ
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
					zmsg_append(reply, &dadaframe);					///�ѻ��͵����ݴ������Ӧ
					int sendFlag = zmsg_send(&reply, sock);		    ///������Ӧ
					index++;
				}
			}
		}
	}
}
*/

/**
* @brief �ش�ң������
* @param[in] sock �׽���
* @param[in] id �׽��ֶ�Ӧ��id
* @param[in] commandid ����ID��
* @return void
*/
//void NodeReplyRemoteData(zsock_t* sock, vector<byte>& id, uint32_t commandid, uint32_t totalbytenum)
//{
//	int totalcount = totalbytenum / sizeof(uint32_t);//�ش�44��uint32_t��������
//	int index = 0;
//	while (index < totalcount)
//	{
//		zmsg_t* reply = zmsg_new();
//		zmsg_addmem(reply, id.data(), id.size());///��peer id�������Ӧ
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
//		zmsg_append(reply, &dadaframe);					///�ѻ��͵����ݴ������Ӧ
//		int sendFlag = zmsg_send(&reply, sock);		    ///������Ӧ
//		index++;
//	}
//}



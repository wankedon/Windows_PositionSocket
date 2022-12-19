#include <iostream>
#include <stdint.h> //������׼��ͷ�ļ�
#include <stdlib.h> //������׼��ͷ�ļ�
#include <windows.h>
#include "PositionSocketAPI.h"  //������̬���е�ͷ�ļ�         
using namespace ZBSYB_RADAR_SOCKET;//Ӧ�������ռ�

//����״̬
bool isconnect = false;

//����ص�����
//1.ͨ��״̬�ص�����
void OnSessionState(ChannelState state, void* userPointer)
{
	if (state == CONNECTED)
	{
		isconnect = true;
	}
	else if (state == DISCONNECTED)
	{
		isconnect = false;
		printf("�Ͽ�����\n");
	}
}

//2.�տػص�����
void OnZKResult(const ZKResult* zkResult, void* userPointer)
{
	if (zkResult->startstopframe != nullptr)
	{
		printf("����/ֹͣ����ش�,����ID��:%0x\n", zkResult->startstopframe->header.commandID);
	}
	else if (zkResult->locktrackframe != nullptr)
	{
		printf("�������ٽ���ش�,����ID��:%0x\n", zkResult->locktrackframe->header.commandID);
	}
	else if (zkResult->selfdetectframe != nullptr)
	{
		printf("�Լ����ش�,����ID��:%0x\n", zkResult->selfdetectframe->header.commandID);
	}
	else
	{
		printf("�޽���ش�......\n");
	}
}

//3.ң��ص�����
void OnYCResult(const YCResult* ycResult, void* userPointer)
{
	if (ycResult != nullptr)
	{
		printf("ң�����ݻش���ң������:remoteData[%d]--remoteData[%zu]\n", 0, ycResult->length - 1);
	}
}

int main()
{
	//����������
	int ps_handler;
	//��ʼ����
	PS_InitSSLib();
	//����ص���������
	SessionHandler callback{ 0 };
	callback.userPointer = (void*)0x1223;
	callback.cbZK.cbCon = OnSessionState; //��ֵΪ�տ�ͨ��״̬�ص�������
	callback.cbZK.cbResult = OnZKResult;  //��ֵΪ�տػص�����
	callback.cbYC.cbCon = OnSessionState; //��ֵΪң��ͨ��״̬�ص�������
	callback.cbYC.cbResult = OnYCResult;  //��ֵΪң��ص�������
	//���ü��ڵ�IP��ַ
	const char* addr = "127.0.1.1:5555";
	//��������
	PS_CreateConnect(&ps_handler, addr, &callback);
	while (true)
	{
		if (isconnect == true)
		{
			printf("���ӳɹ�%s...\n", addr);
			break;
		}
		else
		{
			printf("��������%s...\n", addr);
		}
		Sleep(500);
	}
	//�����Լ�ָ��
	PS_SendSelfDetect(ps_handler);
	system("pause");
	//���Ͳ�������ָ��
	ParamConfFrame paramconfframe;
	ParamConfParam paramconf;
	PS_SendParaConf(&paramconfframe, ps_handler, &paramconf);
	system("pause");
	//��������ָ��
	PS_SendStartStop(ps_handler, 1);
	system("pause");
	//����ָֹͣ��
	PS_SendStartStop(ps_handler, 0);
	system("pause");
	//�ر�����
	PS_CloseConnect(ps_handler);
	//�رտ�
	PS_DeinitSSLib();
	return 0;
}

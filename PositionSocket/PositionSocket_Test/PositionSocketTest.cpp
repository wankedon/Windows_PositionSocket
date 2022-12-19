#include <iostream>
#include <stdint.h> //包含标准库头文件
#include <stdlib.h> //包含标准库头文件
#include <windows.h>
#include "PositionSocketAPI.h"  //包含动态库中的头文件         
using namespace ZBSYB_RADAR_SOCKET;//应用命名空间

//连接状态
bool isconnect = false;

//定义回调函数
//1.通道状态回调函数
void OnSessionState(ChannelState state, void* userPointer)
{
	if (state == CONNECTED)
	{
		isconnect = true;
	}
	else if (state == DISCONNECTED)
	{
		isconnect = false;
		printf("断开连接\n");
	}
}

//2.终控回调函数
void OnZKResult(const ZKResult* zkResult, void* userPointer)
{
	if (zkResult->startstopframe != nullptr)
	{
		printf("启动/停止结果回传,命令ID号:%0x\n", zkResult->startstopframe->header.commandID);
	}
	else if (zkResult->locktrackframe != nullptr)
	{
		printf("锁定跟踪结果回传,命令ID号:%0x\n", zkResult->locktrackframe->header.commandID);
	}
	else if (zkResult->selfdetectframe != nullptr)
	{
		printf("自检结果回传,命令ID号:%0x\n", zkResult->selfdetectframe->header.commandID);
	}
	else
	{
		printf("无结果回传......\n");
	}
}

//3.遥测回调函数
void OnYCResult(const YCResult* ycResult, void* userPointer)
{
	if (ycResult != nullptr)
	{
		printf("遥测数据回传，遥测数据:remoteData[%d]--remoteData[%zu]\n", 0, ycResult->length - 1);
	}
}

int main()
{
	//定义句柄变量
	int ps_handler;
	//初始化库
	PS_InitSSLib();
	//定义回调函数集合
	SessionHandler callback{ 0 };
	callback.userPointer = (void*)0x1223;
	callback.cbZK.cbCon = OnSessionState; //赋值为终控通道状态回调函数名
	callback.cbZK.cbResult = OnZKResult;  //赋值为终控回调函数
	callback.cbYC.cbCon = OnSessionState; //赋值为遥测通道状态回调函数名
	callback.cbYC.cbResult = OnYCResult;  //赋值为遥测回调函数名
	//设置监测节点IP地址
	const char* addr = "127.0.1.1:5555";
	//建立连接
	PS_CreateConnect(&ps_handler, addr, &callback);
	while (true)
	{
		if (isconnect == true)
		{
			printf("连接成功%s...\n", addr);
			break;
		}
		else
		{
			printf("尝试连接%s...\n", addr);
		}
		Sleep(500);
	}
	//发送自检指令
	PS_SendSelfDetect(ps_handler);
	system("pause");
	//发送参数配置指令
	ParamConfFrame paramconfframe;
	ParamConfParam paramconf;
	PS_SendParaConf(&paramconfframe, ps_handler, &paramconf);
	system("pause");
	//发送启动指令
	PS_SendStartStop(ps_handler, 1);
	system("pause");
	//发送停止指令
	PS_SendStartStop(ps_handler, 0);
	system("pause");
	//关闭连接
	PS_CloseConnect(ps_handler);
	//关闭库
	PS_DeinitSSLib();
	return 0;
}

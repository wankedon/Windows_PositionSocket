/**
* @file YCChannel.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief YCChannel类所在头文件

* @author wangkedong

* @date 2020:06:04

* @note 遥测通路YCChannel继承自DeviceChannel

* @version <1.0.0>

*/
#pragma once
#include "DeviceChannel.h"
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief 遥测通路YCChannel继承自DeviceChannel
	* @details
	*/
	class YCChannel :
		public DeviceChannel
	{
	public:
		YCChannel();
		virtual ~YCChannel();
	public:
		PS_Error CreateConnect(const char* peerAddress, const YCCallbacks& yccallbacks, void* userPointer);
		static void actorFuncWrap(zsock_t* pipe, void* args);
	private:
		static int pipeEventHandler(zloop_t* loop, zsock_t* handle, void* arg);
		static int timeHandler(zloop_t* loop, int timerId, void* arg);//20201124wkd
		void ontimer();//20201124wkd
		void startTimer(size_t interval, size_t times);
		void stopTimer();//20201124wkd
		virtual void SS_OnReceiveComplete(const DataBlock& db);
		void actorFunc(zsock_t* pipe);
	private:
		zloop_t* zloop;
		int timerId;
		uint32_t clientLiveness;
		unique_ptr<thread> m_thread;
		unique_ptr<DeviceStreamBuffer> m_buffer;
		function<void(const YCResult* ycResult)> m_ycResultHandler;
		zactor_t* actor;
	};
}
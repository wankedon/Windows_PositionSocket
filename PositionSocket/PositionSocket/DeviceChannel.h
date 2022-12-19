/**
* @file DeviceChannel.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief DeviceChannel类所在头文件

* @author wangkedong

* @date 2020:07:09

* @note 作为ZKChannel与YCChannel的基类 调用SuperSocket函数 建立连接 接收回调结果

* @version <1.0.0>

*/
#pragma once
#include "PositionSocketAPI.h"
#include "SuperSocketTransport.h"
#include "DeviceStreamBuffer.h"
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief DeviceChannel基类 调用SuperSocket函数 建立连接 接收回调结果
	* @details
	*/
	class DeviceChannel
	{
	public:
		DeviceChannel(const vector<byte>& channelWord);
		virtual ~DeviceChannel();
	public:
		PS_Error CreateConnect(const char* peerAddress, CHANNEL_STATE_CALLBACK* cbCon, void* userPointer);
	protected:
		enum ConnectStatus
		{
			UNCONNECTED = 0,
			CONNECT,
			CUTOFF,
		};
	private:
		static void SS_OnConnectCompleteCallback(void* userPointer);
		static void SS_OnAlreadyDisConnectCallback(void* userPointer);
		static void SS_OnSendCompleteCallback(SSError err, void* userPointer);
		static void SS_OnReceiveCompleteCallback(const DataBlock& db, void* userPointer);
		void SS_OnConnectComplete();
		//void SS_OnAlreadyDisConnect();//20201124wkd
		void SS_OnSendComplete(SSError err);
	protected:
		void SS_OnAlreadyDisConnect();//20201124wkd
		virtual void SS_OnReceiveComplete(const DataBlock& db) = 0;
	protected:
		ConnectStatus m_status;
		const vector<byte> m_channelWord;
		unique_ptr<SuperSocketTransport> m_transport;
		function<void(ChannelState channelstate)> m_channelStateHandler;
	};
}
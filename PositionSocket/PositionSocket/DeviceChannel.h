/**
* @file DeviceChannel.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief DeviceChannel������ͷ�ļ�

* @author wangkedong

* @date 2020:07:09

* @note ��ΪZKChannel��YCChannel�Ļ��� ����SuperSocket���� �������� ���ջص����

* @version <1.0.0>

*/
#pragma once
#include "PositionSocketAPI.h"
#include "SuperSocketTransport.h"
#include "DeviceStreamBuffer.h"
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief DeviceChannel���� ����SuperSocket���� �������� ���ջص����
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
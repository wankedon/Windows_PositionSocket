#include "stdafx.h"
#include "DeviceChannel.h"
namespace ZBSYB_RADAR_SOCKET
{
	DeviceChannel::DeviceChannel(const vector<byte>& channelWord)
		:m_status(UNCONNECTED),
		m_channelWord(channelWord)
	{
		m_transport = make_unique<SuperSocketTransport>();
	}

	DeviceChannel::~DeviceChannel()
	{
	}

	/**
	* @brief 建立连接
	* @return PS_Error
	*/
	PS_Error DeviceChannel::CreateConnect(const char* peerAddress, CHANNEL_STATE_CALLBACK* cbCon, void* userPointer)
	{
		m_channelStateHandler = std::bind(cbCon, std::placeholders::_1, userPointer);
		SSEventCallback ss_callback;
		ss_callback.userPointer = this;
		ss_callback.cbConnect = SS_OnConnectCompleteCallback;
		ss_callback.cbDisconnect = SS_OnAlreadyDisConnectCallback;
		ss_callback.cbSend = SS_OnSendCompleteCallback;
		ss_callback.cbReceive = SS_OnReceiveCompleteCallback;
		return m_transport->CreateSocket(peerAddress, ss_callback) == PS_ERR_NONE ? PS_ERR_NONE : PS_ERR_CONNECT;
	}

	/**
	* @brief 连接回调
	* @return PS_Error
	*/
	void DeviceChannel::SS_OnConnectCompleteCallback(void* userPointer)
	{
		DeviceChannel* self = (DeviceChannel*)userPointer;
		self->SS_OnConnectComplete();
	}

	void DeviceChannel::SS_OnConnectComplete()
	{
		m_status = CONNECT;
		if (m_channelStateHandler)  ///通过回调通知用户
		{
			m_channelStateHandler(CONNECTED);
		}
		DataBlock db{ m_channelWord.size(), const_cast<byte*> (&m_channelWord[0]) };
		m_transport->Send(db);
	}

	/**
	* @brief 断开连接回调
	* @return PS_Error
	*/
	void DeviceChannel::SS_OnAlreadyDisConnectCallback(void* userPointer)
	{
		DeviceChannel* self = (DeviceChannel*)userPointer;
		self->SS_OnAlreadyDisConnect();
	}

	void DeviceChannel::SS_OnAlreadyDisConnect()
	{
		m_status = CUTOFF;
		if (m_channelStateHandler)  ///通过回调通知用户
		{
			m_channelStateHandler(DISCONNECTED);
		}
	}

	/**
	* @brief 发送回调
	* @return PS_Error
	*/
	void DeviceChannel::SS_OnSendCompleteCallback(SSError err, void* userPointer)
	{
		DeviceChannel* self = (DeviceChannel*)userPointer;
		self->SS_OnSendComplete(err);
	}

	void DeviceChannel::SS_OnSendComplete(SSError err)
	{
		if (err != 0)
			zsys_debug("send complete, error code = %d", err);
	}

	/**
	* @brief 接收回调
	* @return PS_Error
	*/
	void DeviceChannel::SS_OnReceiveCompleteCallback(const DataBlock& db, void* userPointer)
	{
		DeviceChannel* self = (DeviceChannel*)userPointer;
		self->SS_OnReceiveComplete(db);
	}
}
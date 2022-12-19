#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "SuperSocketTransport.h"
namespace ZBSYB_RADAR_SOCKET
{
	SuperSocketTransport::SuperSocketTransport()
		:m_socketHandle(0)
	{
	}

	SuperSocketTransport::~SuperSocketTransport()
	{
		DestroySocket();
	}

	PS_Error SuperSocketTransport::CreateSocket(const char* peerAddress, SSEventCallback& callback)
	{
		auto error = SS_CreateSocket(&m_socketHandle, peerAddress, &callback);///创建一个socket，建立一个连接
		if (error != SS_ERR_NONE)
			return PS_ERR_CREATE_SOCKET;
		return PS_ERR_NONE;
	}

	PS_Error SuperSocketTransport::Send(const DataBlock& db)
	{
		auto error = SS_SendBlock(m_socketHandle, db);
		if (error != SS_ERR_NONE)
		{
			zsys_debug("Send Fail");
			return PS_ERR_SEND;
		}
		return PS_ERR_NONE;
	}


	void SuperSocketTransport::DestroySocket()
	{
		if (m_socketHandle != 0)
		{
			SS_DestroySocket(m_socketHandle);
			m_socketHandle = 0;
		}
	}
}
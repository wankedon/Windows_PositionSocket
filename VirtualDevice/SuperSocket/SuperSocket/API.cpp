#include "stdafx.h"
#include "SuperSocketAPI.h"
#include "HandlePool.h"

namespace SuperSocketLib
{

	unique_ptr<HandlePool> uniquePool;
	SUPERSOCKET_API SSErrorCode SS_InitSSLib()
	{
		zsys_init();
		if (uniquePool == nullptr)
		{
			uniquePool = make_unique<HandlePool>();	///<±ÜÃâÖØ¸´´´½¨
		}
		return SS_ERR_NONE;
	}

	SUPERSOCKET_API SSErrorCode SS_DeinitSSLib()
	{
		uniquePool.reset();
		zsys_shutdown();
		return SS_ERR_NONE;
	}

	SUPERSOCKET_API SSErrorCode SS_CreateServerSocket(int* fd, const char* listenAddr, const EventHandler* handler)
	{
		if (uniquePool == nullptr)
			return SS_ERR_INIT;
		string addr(listenAddr);
		*fd = uniquePool->newSocket(addr, handler, false);
		return *fd > 0 ? SS_ERR_NONE : SS_ERR_CREATE;
	}

	SUPERSOCKET_API SSErrorCode SS_CreateClientSocket(int* fd, const char* peerAddr, const EventHandler* handler)
	{
		if (uniquePool == nullptr)
			return SS_ERR_INIT;
		string addr(peerAddr);
		*fd = uniquePool->newSocket(addr, handler, true);
		return *fd > 0 ? SS_ERR_NONE : SS_ERR_CREATE;
	}

	SUPERSOCKET_API SSErrorCode SS_DestroySocket(int fd)
	{
		if (uniquePool == nullptr)
			return SS_ERR_INIT;
		return uniquePool->closeHandle(fd);
	}

	SUPERSOCKET_API SSErrorCode SS_SendBlock(int fd, const NetBlock* data, const NodeId* to)
	{
		if (uniquePool == nullptr)
			return SS_ERR_INIT;
		return uniquePool->send(fd, data, to);
	}
}

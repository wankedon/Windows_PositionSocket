/**
 * @file HandlePool.cpp
 
 * @brief HandlePool��ʵ��
 
 * @author yangqing
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ�� All Rights Reserved
 
 * @date 2020:03:25
 
 * @note include ·��������vcpkg
 
 * @version <1.0.0>
 
 */
#include "stdafx.h"
#include "HandlePool.h"
#include "SuperSocket.h"
#include <math.h>
namespace SuperSocketLib
{
	HandlePool::HandlePool()
	{
		for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
		{
			_spareHandles.push_back(0xCE41 + i);
		}
	}


	HandlePool::~HandlePool()
	{
	}

	int HandlePool::newSocket(const std::string& address, const EventHandler* handler, bool client)
	{
		if (_spareHandles.empty())
			return -1;
		int handle = _spareHandles.front();
		assert(_socketsInUse.find(handle) == _socketsInUse.end());
		_socketsInUse[handle] = make_shared<SuperSocket>(address, handler, client);
		_spareHandles.pop_front();
		return handle;
	}

	SSErrorCode HandlePool::send(int handle, const NetBlock* block, const NodeId* to)
	{
		if (_socketsInUse.find(handle) == _socketsInUse.end())
			return SS_ERR_INVALID_HANDLE;
		return _socketsInUse[handle]->send(block, to);
	}

	SSErrorCode HandlePool::closeHandle(int handle)
	{
		if (_socketsInUse.find(handle) == _socketsInUse.end())
			return SS_ERR_INVALID_HANDLE;
		_socketsInUse.erase(handle);			///< �ų�socket���Զ�����
		_spareHandles.push_back(handle);		///< �黹�����
		return SS_ERR_NONE;
	}
}

/**
 * @file SuperSocket.cpp
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief SuperSocket类的实现
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#include "stdafx.h"
#include "SuperSocket.h"
#include "SuperSocketImpl.h"

namespace SuperSocketLib
{
	SuperSocket::SuperSocket(const std::string& address, const EventHandler* handler, bool client)
	{
		_impl = new SuperSocketImpl(address, handler, client);
	}

	SuperSocket::~SuperSocket()
	{
		close();
	}

	SSErrorCode SuperSocket::send(const NetBlock* block, const NodeId* to)
	{
		return _impl->send(block, to);
	}

	void SuperSocket::close()
	{
		if (_impl)
		{
			delete _impl;
			_impl = nullptr;
		}
	}
}

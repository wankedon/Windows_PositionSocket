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

SuperSocket::SuperSocket(const std::string& peerAddress, const SSEventCallback& callbacks)
{
	_impl = new SuperSocketImpl(peerAddress, callbacks); ///新建对象
}

SuperSocket::~SuperSocket()
{
	close();///析构函数中删除新建对象
}

SSError SuperSocket::send(const DataBlock& block)
{
	return _impl->send(block);
}

void SuperSocket::close()
{
	if (_impl)
	{
		delete _impl; ///删除新建对象
		_impl = nullptr;
	}
}

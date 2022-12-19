/**
 * @file HandlePool.cpp
 
 * @brief HandlePool的实现
 
 * @author yangqing
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部 All Rights Reserved
 
 * @date 2020:03:25
 
 * @note include 路径包含了vcpkg
 
 * @version <1.0.0>
 
 */
#include "stdafx.h"
#include "HandlePool.h"
#include "SuperSocket.h"
#include <math.h>

HandlePool::HandlePool()
{
	for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
	{
		_spareHandles.push_back(0xCE41 + i);      ///构造函数中构建MAX_HANDLE_COUNT个备用句柄
	}
}

HandlePool::~HandlePool()
{
}

int HandlePool::newHandle(const std::string& peerAddress, const SSEventCallback& callbacks)
{
	if (_spareHandles.empty()) ///list必须不为空 否则 pop_front()崩溃
		return -1;
	int handle = _spareHandles.front(); ///获得备用句柄中的第一个
	assert(_socketsInUse.find(handle) == _socketsInUse.end()); ///handle不存在向下执行
	_socketsInUse[handle] = make_shared<SuperSocket>(peerAddress, callbacks);/// 第一个备用句柄 转为在用
	_spareHandles.pop_front(); ///擦除备用句柄中的第一个
	return handle;
}

SSError HandlePool::send(int handle, const DataBlock& block)
{
	if (_socketsInUse.find(handle) == _socketsInUse.end())///handle不存在
		return SS_ERR_INVALID_HANDLE;
	return _socketsInUse[handle]->send(block);///通过键handle找到 对象 访问send方法 发送数据
}

SSError HandlePool::closeHandle(int handle)
{
	if (_socketsInUse.find(handle) == _socketsInUse.end())
		return SS_ERR_INVALID_HANDLE;
	_socketsInUse.erase(handle);			///< 排除socket，自动清理
	_spareHandles.push_back(handle);		///< 归还句柄号
	return SS_ERR_NONE;
}

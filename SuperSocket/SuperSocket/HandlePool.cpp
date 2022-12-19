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

HandlePool::HandlePool()
{
	for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
	{
		_spareHandles.push_back(0xCE41 + i);      ///���캯���й���MAX_HANDLE_COUNT�����þ��
	}
}

HandlePool::~HandlePool()
{
}

int HandlePool::newHandle(const std::string& peerAddress, const SSEventCallback& callbacks)
{
	if (_spareHandles.empty()) ///list���벻Ϊ�� ���� pop_front()����
		return -1;
	int handle = _spareHandles.front(); ///��ñ��þ���еĵ�һ��
	assert(_socketsInUse.find(handle) == _socketsInUse.end()); ///handle����������ִ��
	_socketsInUse[handle] = make_shared<SuperSocket>(peerAddress, callbacks);/// ��һ�����þ�� תΪ����
	_spareHandles.pop_front(); ///�������þ���еĵ�һ��
	return handle;
}

SSError HandlePool::send(int handle, const DataBlock& block)
{
	if (_socketsInUse.find(handle) == _socketsInUse.end())///handle������
		return SS_ERR_INVALID_HANDLE;
	return _socketsInUse[handle]->send(block);///ͨ����handle�ҵ� ���� ����send���� ��������
}

SSError HandlePool::closeHandle(int handle)
{
	if (_socketsInUse.find(handle) == _socketsInUse.end())
		return SS_ERR_INVALID_HANDLE;
	_socketsInUse.erase(handle);			///< �ų�socket���Զ�����
	_spareHandles.push_back(handle);		///< �黹�����
	return SS_ERR_NONE;
}

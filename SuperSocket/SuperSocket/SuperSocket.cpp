/**
 * @file SuperSocket.cpp
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
 * ��Ȩ���У�δ����ɣ����ø��ơ�������ת��
 
 * @brief SuperSocket���ʵ��
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��
 
 * @version <1.0.0>
 
 */
#include "stdafx.h"
#include "SuperSocket.h"
#include "SuperSocketImpl.h"

SuperSocket::SuperSocket(const std::string& peerAddress, const SSEventCallback& callbacks)
{
	_impl = new SuperSocketImpl(peerAddress, callbacks); ///�½�����
}

SuperSocket::~SuperSocket()
{
	close();///����������ɾ���½�����
}

SSError SuperSocket::send(const DataBlock& block)
{
	return _impl->send(block);
}

void SuperSocket::close()
{
	if (_impl)
	{
		delete _impl; ///ɾ���½�����
		_impl = nullptr;
	}
}

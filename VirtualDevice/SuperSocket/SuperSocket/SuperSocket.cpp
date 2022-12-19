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

/**
 * @file HandlePool.h
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
 * ��Ȩ���У�δ����ɣ����ø��ơ�������ת��
 
 * @brief �����Դ��
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��
 
 * @version <1.0.0>
 
 */
#pragma once

#include "SuperSocketAPI.h"
/**
 * @brief �����Դ��
 *
 */
namespace SuperSocketLib
{
class SuperSocket;
class HandlePool
{
public:
	HandlePool();
	~HandlePool();
public:
	/**
	 * @brief �����µ�����
	 * @details
	 * @param[in] peerAddress Զ��������ַ
	 * @param[in] callbacks �ص���������
	 * @return int 
	 * @retval �¾��
	 */
	int newSocket(const std::string& address, const EventHandler* handler, bool client);
	/**
	 * @brief ��������
	 * @details
	 * @param[in] handle socket���
	 * @param[in] block �����͵�����
	 * @return SSError
	 */
	SSErrorCode send(int handle, const NetBlock* block, const NodeId* to);
	/**
	 * @brief �رվ��
	 * @details
	 * @param[int] handle socket���
	 * @return SSError
	 */
	SSErrorCode closeHandle(int handle);
private:
	map<int, shared_ptr<SuperSocket>> _socketsInUse;	///<���õ�socket
	list<int> _spareHandles;		///<���þ��
	const int MAX_HANDLE_COUNT = 64;	///<�����Դ����
	const int MAGIC_NUM = 0xCE41;	///<�����ʶ����
};

}


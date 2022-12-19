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
	int newHandle(const std::string& peerAddress, const SSEventCallback& callbacks);
	/**
	 * @brief ��������
	 * @details
	 * @param[in] handle socket���
	 * @param[in] block �����͵�����
	 * @return SSError
	 */
	SSError send(int handle, const DataBlock& block);
	/**
	 * @brief �رվ��
	 * @details
	 * @param[int] handle socket���
	 * @return SSError
	 */
	SSError closeHandle(int handle);
private:
	map<int, shared_ptr<SuperSocket>> _socketsInUse;	///<���õ�socket  ��ֵ��map
	list<int> _spareHandles;		///<���þ��
	const int MAX_HANDLE_COUNT = 256;///<�����Դ����
	const int MAGIC_NUM = 0xCE41;	///<�����ʶ����
};


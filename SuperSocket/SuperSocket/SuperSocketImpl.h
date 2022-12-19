/**
 * @file SuperSocketImpl.h
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
 * ��Ȩ���У�δ����ɣ����ø��ơ�������ת��
 
 * @brief SuperSocketImpl���ڵ�ͷ�ļ�
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��
 
 * @version <1.0.0>
 
 */
#pragma once

#include "Actor.h"
#include "SuperSocketAPI.h"

struct TransferBuffer; ///<�����ýṹ��

/**
 * @brief SuperSocket�ľ���ʵ��
 *
 */
class SuperSocketImpl : public Actor
{
public:
	SuperSocketImpl(const string& peerAddress, const SSEventCallback& callbacks); 
	virtual ~SuperSocketImpl();

public:
	SSError send(const DataBlock& block);     

private:
	static int streamSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg);  
	static int pipeSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg);    
	virtual void actorFunc(zsock_t *pipe);    
	void innerSend(zframe_t** f);             
	void onStreamEvent(zsock_t *handle);      

private:
	const string  _peerAddress;            
	zsock_t* _streamSock;                     
	vector<byte> _identity;                  
	std::function<void(SSError, void* userPointer)> _sndHandler; 
	function<void(const DataBlock&, void* userPointer)> _rcvHandler;
	function<void(void* userPointer)> _connectHandler;
	function<void(void* userPointer)> _disconnectHandler;
	void* _userPointer;
};



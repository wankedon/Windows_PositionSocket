/**
 * @file SuperSocketImpl.h
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
 * ��Ȩ���У�δ����ɣ����ø��ơ�������ת��
 
 * @brief 
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��
 
 * @version <1.0.0>
 
 */
#pragma once

#include "Actor.h"
#include "SuperSocketAPI.h"

struct TransferBuffer; ///<�����ýṹ��
using namespace SuperSocketLib;
/**
 * @brief SuperSocket�ľ���ʵ��
 *
 */
class SuperSocketImpl : public Actor
{
public:
	SuperSocketImpl(const std::string& address, const EventHandler* handler, bool client);
	virtual ~SuperSocketImpl();
public:
	SSErrorCode send(const NetBlock* block, const NodeId* to);
private:
	static int streamSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg);
	static int pipeSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg);
	virtual void actorFunc(zsock_t *pipe);
	void innerSend(zmsg_t** msg);
	void onStreamEvent(zsock_t *handle);
	NodeId getMsgId(zmsg_t* msg);
private:
	const string _address;
	const bool _client;
	zsock_t* _streamSock;
	set<NodeId> nodes;
	function<void(SSErrorCode, const NodeId*)> _sndHandler;
	function<void(const NetBlock*, const NodeId*)> _rcvHandler;
	function<void(const NodeId*)> _connectHandler;
	function<void(const NodeId*)> _disconnectHandler;
};


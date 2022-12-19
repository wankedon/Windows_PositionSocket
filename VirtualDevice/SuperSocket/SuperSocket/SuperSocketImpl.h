/**
 * @file SuperSocketImpl.h
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief 
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#pragma once

#include "Actor.h"
#include "SuperSocketAPI.h"

struct TransferBuffer; ///<声明该结构体
using namespace SuperSocketLib;
/**
 * @brief SuperSocket的具体实现
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


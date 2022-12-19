/**
 * @file SuperSocketImpl.h
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief SuperSocketImpl所在的头文件
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#pragma once

#include "Actor.h"
#include "SuperSocketAPI.h"

struct TransferBuffer; ///<声明该结构体

/**
 * @brief SuperSocket的具体实现
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



#include "stdafx.h"
#include "SuperSocketImpl.h"
#include "SuperSocket.h"

SuperSocketImpl::SuperSocketImpl(const string& peerAddress, const SSEventCallback& callbacks)
	:_peerAddress(peerAddress),
	 _streamSock(nullptr),
	 _sndHandler(callbacks.cbSend),
	 _rcvHandler(callbacks.cbReceive),
	 _connectHandler(callbacks.cbConnect),
	 _disconnectHandler(callbacks.cbDisconnect),
	 _userPointer(callbacks.userPointer)
{
	actor = zactor_new(actorFuncWrap, this);
}

SuperSocketImpl::~SuperSocketImpl()
{
	zactor_destroy(&actor);
}

/**
 * @brief 发送数据块
 * @details 公开方法，用户代码调用该函数发送数据块
 * @param block 待发送的数据块
 * @return errorCode
 * @retval SS_ERR_NONE,SS_ERR_SEND
 */
SSError SuperSocketImpl::send(const DataBlock& block)
{
	zframe_t* f = zframe_new(block.addr, block.len);
	return zframe_send(&f, actor, 0) == 0 ? SS_ERR_NONE : SS_ERR_SEND;
}

/**
 * @brief  actor内部执行的函数
 * @detail 利用zloop监听pipe, stream的事件，并处理
 * @param pipe pair-socket管线
 * @return 
 * @retval 
 */
void SuperSocketImpl::actorFunc(zsock_t *pipe)
{
	Actor::actorFunc(pipe);      
	zloop_t* zloop = zloop_new();
	int rc = zloop_reader(zloop, pipe, pipeSockEventHandler, this);      
	zloop_reader_set_tolerant(zloop, pipe);
	_streamSock = zsock_new(ZMQ_STREAM);
	rc = zsock_connect(_streamSock, "tcp://%s", _peerAddress.c_str());
	rc = zloop_reader(zloop, _streamSock, streamSockEventHandler, this);  
	zloop_reader_set_tolerant(zloop, _streamSock);
	zloop_start(zloop);		
	zloop_destroy(&zloop);
	zsock_destroy(&_streamSock);
}

int SuperSocketImpl::streamSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	SuperSocketImpl* self = (SuperSocketImpl*)arg;
	self->onStreamEvent(handle);	
	return 0;
}

void SuperSocketImpl::onStreamEvent(zsock_t *handle)
{
	assert(handle == _streamSock);
	zmsg_t* msg = zmsg_recv(handle);
	assert(zmsg_size(msg) == 2);
	zframe_t* fid = zmsg_first(msg);	///< 第一帧， id
	zframe_t* fBlock = zmsg_next(msg);	///< 第二帧，载荷数据块
	int dataBlockSize = zframe_size(fBlock);
	if (dataBlockSize)  ///<第二帧不为空帧，接收到数据
	{
		DataBlock block{ dataBlockSize, zframe_data(fBlock) };
		if (_rcvHandler)
		{
			_rcvHandler(block, _userPointer);	///<回调，向用户传送数据
		}
	}
	else ///<第二帧为空帧，有特殊事件发生，例如连接或断开
	{
		int idSize = zframe_size(fid);
		if (_identity.empty())  ///<储存id号的变量为空，此时连接未建立，则记录建立连接并记录id 
		{
			_identity.resize(idSize);
			memcpy(_identity.data(), zframe_data(fid), idSize);	///<记录id，以备发送时用
			if (_connectHandler)
				_connectHandler(_userPointer);///<回调，连接
		}
		else ///<储存id号的变量不为空，表明此时连接已建立，则断开连接，清理储存id号的变量
		{
			//zsys_info("disconnected");
			_identity.clear();
			if (_disconnectHandler)
				_disconnectHandler(_userPointer);///<回调，未建立链接
		}
	}
	zmsg_destroy(&msg);	///<最终销毁该消息
}

/**
 * @brief actor管道处理函数 
 * @detail
 * @param loop zloop对象
 * @param handle pipe socket
 * @param arg this 指针
 * @return 0
 * @retval 
 */
int SuperSocketImpl::pipeSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	static const char* TERM = "$TERM";
	SuperSocketImpl* self = (SuperSocketImpl*)arg;
	zframe_t* f = zframe_recv(handle);          ///后台从pipe管道接收数据
	if (memcmp(TERM, zframe_data(f), 5) == 0)	///<退出的信息标识，返回-1，zloop将退出 memcmp比较
		return -1;
	self->innerSend(&f);	                    ///<将从pipe管道获得的msg的转发到_streamSock
	return 0;
}

/**
 * @brief 消息转发
 * @details 把从pipe(即用户代码)发来的frame，经_streamSock发送给peer
 * @param[in,out] zframe_t** 用户发来的消息帧，打包到消息后自动销毁
 * @return 
 * @retval 
 */
void SuperSocketImpl::innerSend(zframe_t** f)
{
	SSError err = SS_ERR_NONE;
	if (_identity.empty())    ///若id为空帧，尚未建立有效连接
	{
		err = SS_ERR_NOT_CONNECTED;
	}
	else                     ///若建立了有效连接，发数据到_streamSock
	{
		zmsg_t* msg = zmsg_new();
		zmsg_addmem(msg, _identity.data(), _identity.size());
		zmsg_append(msg, f);
		auto rc = zmsg_send(&msg, _streamSock);
		if (rc != 0)
			err = SS_ERR_SEND;
	}
	if (_sndHandler)
		_sndHandler(err, _userPointer);///<发送数据回调
}
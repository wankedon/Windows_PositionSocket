#include "stdafx.h"
#include "SuperSocketImpl.h"
#include "SuperSocket.h"

SuperSocketImpl::SuperSocketImpl(const std::string& address, const EventHandler* handler, bool client)
	:_address(address),
	_client(client),
	_streamSock(nullptr)
{
	_sndHandler = std::bind(handler->cbSend, std::placeholders::_1, std::placeholders::_2, handler->userPointer);
	_rcvHandler = std::bind(handler->cbReceive, std::placeholders::_1, std::placeholders::_2, handler->userPointer);
	_connectHandler = std::bind(handler->cbConnect, std::placeholders::_1, handler->userPointer);
	_disconnectHandler = std::bind(handler->cbDisconnect, std::placeholders::_1, handler->userPointer);
	actor = zactor_new(actorFuncWrap, this);
}

SuperSocketImpl::~SuperSocketImpl()
{
	zactor_destroy(&actor);
	zsys_info("superSocket destroyed");
}

/**
 * @brief 发送数据块
 * @details 公开方法，用户代码调用该函数发送数据块
 * @param block 待发送的数据块
 * @return errorCode
 * @retval SS_ERR_NONE,SS_ERR_SEND
 */
SSErrorCode SuperSocketImpl::send(const NetBlock* block, const NodeId* to)
{
	zmsg_t* msg = zmsg_new();
	zmsg_addmem(msg, to->id, to->idLength);
	zmsg_addmem(msg, block->addr, block->len);
	return zmsg_send(&msg, actor) == 0 ? SS_ERR_NONE : SS_ERR_SEND;
}

/**
 * @brief actor内部执行的函数
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
	if (_client)
		rc = zsock_connect(_streamSock, "tcp://%s", _address.c_str());
	else
		rc = zsock_bind(_streamSock, "tcp://%s", _address.c_str());
	rc = zloop_reader(zloop, _streamSock, streamSockEventHandler, this);
	zloop_reader_set_tolerant(zloop, _streamSock);
	zloop_start(zloop);		///<启动reactor
	zloop_destroy(&zloop);	///<循环结束后清理
	zsock_destroy(&_streamSock);
}

int SuperSocketImpl::streamSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	SuperSocketImpl* self = (SuperSocketImpl*)arg;
	self->onStreamEvent(handle);
	return 0;
}

NodeId SuperSocketImpl::getMsgId(zmsg_t* msg)
{
	zframe_t* fid = zmsg_first(msg);	///< 第一帧， id
	return NodeId{ zframe_data(fid), zframe_size(fid) };
}

void SuperSocketImpl::onStreamEvent(zsock_t *handle)
{
	assert(handle == _streamSock);
	zmsg_t* msg = zmsg_recv(handle);
	assert(zmsg_size(msg) == 2);
	NodeId id = getMsgId(msg);
	zframe_t* fBlock = zmsg_next(msg);	///< 第二帧，载荷数据块
	auto dataBlockSize = zframe_size(fBlock);
	if (dataBlockSize)
	{
		zsys_debug("receive %d bytes", dataBlockSize);
		NetBlock block{ dataBlockSize, zframe_data(fBlock) };
		if (_rcvHandler)
		{
			_rcvHandler(&block, &id);	///<回调，向用户传送数据
		}
	}
	else ///<第二帧为空帧，有特殊事件发生，例如连接或断开
	{
		if (nodes.find(id) == nodes.end())
		{
			zsys_debug("%s connected", id.id);
			nodes.insert(id);
			if (_connectHandler)
				_connectHandler(&id);
		}
		else
		{
			zsys_info("%s disconnected", id.id);
			nodes.erase(id);
			if (_disconnectHandler)
				_disconnectHandler(&id);
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
	zmsg_t* msg = zmsg_recv(handle);
	int rc = 0;
	if (zmsg_size(msg) == 1)
	{
		zframe_t* f = zmsg_first(msg);
		if (memcmp(TERM, zframe_data(f), 5) == 0)	///<退出的信息标识，返回-1，zloop将退出
			rc = -1;
		else
			zsys_debug("unkown pip message");
		zmsg_destroy(&msg);
	}
	else
	{
		self->innerSend(&msg);	///<从pipe管道获得msg，转发之
	}
	return rc;
}

/**
 * @brief 消息转发
 * @details 把从pipe(即用户代码)发来的frame，经_streamSock发送给peer
 * @param[in,out] zframe_t** 用户发来的消息帧，打包到消息后自动销毁
 * @return
 * @retval
 */
void SuperSocketImpl::innerSend(zmsg_t** msg)
{
	SSErrorCode err = SS_ERR_SEND;
	NodeId id = getMsgId(*msg);
	if (nodes.find(id) != nodes.end())
	{
		if (zmsg_send(msg, _streamSock) == 0)
		{
			err = SS_ERR_NONE;
		}
	}
	if (_sndHandler)
		_sndHandler(err, &id);
}



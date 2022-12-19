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
 * @brief �������ݿ�
 * @details �����������û�������øú����������ݿ�
 * @param block �����͵����ݿ�
 * @return errorCode
 * @retval SS_ERR_NONE,SS_ERR_SEND
 */
SSError SuperSocketImpl::send(const DataBlock& block)
{
	zframe_t* f = zframe_new(block.addr, block.len);
	return zframe_send(&f, actor, 0) == 0 ? SS_ERR_NONE : SS_ERR_SEND;
}

/**
 * @brief  actor�ڲ�ִ�еĺ���
 * @detail ����zloop����pipe, stream���¼���������
 * @param pipe pair-socket����
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
	zframe_t* fid = zmsg_first(msg);	///< ��һ֡�� id
	zframe_t* fBlock = zmsg_next(msg);	///< �ڶ�֡���غ����ݿ�
	int dataBlockSize = zframe_size(fBlock);
	if (dataBlockSize)  ///<�ڶ�֡��Ϊ��֡�����յ�����
	{
		DataBlock block{ dataBlockSize, zframe_data(fBlock) };
		if (_rcvHandler)
		{
			_rcvHandler(block, _userPointer);	///<�ص������û���������
		}
	}
	else ///<�ڶ�֡Ϊ��֡���������¼��������������ӻ�Ͽ�
	{
		int idSize = zframe_size(fid);
		if (_identity.empty())  ///<����id�ŵı���Ϊ�գ���ʱ����δ���������¼�������Ӳ���¼id 
		{
			_identity.resize(idSize);
			memcpy(_identity.data(), zframe_data(fid), idSize);	///<��¼id���Ա�����ʱ��
			if (_connectHandler)
				_connectHandler(_userPointer);///<�ص�������
		}
		else ///<����id�ŵı�����Ϊ�գ�������ʱ�����ѽ�������Ͽ����ӣ�������id�ŵı���
		{
			//zsys_info("disconnected");
			_identity.clear();
			if (_disconnectHandler)
				_disconnectHandler(_userPointer);///<�ص���δ��������
		}
	}
	zmsg_destroy(&msg);	///<�������ٸ���Ϣ
}

/**
 * @brief actor�ܵ������� 
 * @detail
 * @param loop zloop����
 * @param handle pipe socket
 * @param arg this ָ��
 * @return 0
 * @retval 
 */
int SuperSocketImpl::pipeSockEventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	static const char* TERM = "$TERM";
	SuperSocketImpl* self = (SuperSocketImpl*)arg;
	zframe_t* f = zframe_recv(handle);          ///��̨��pipe�ܵ���������
	if (memcmp(TERM, zframe_data(f), 5) == 0)	///<�˳�����Ϣ��ʶ������-1��zloop���˳� memcmp�Ƚ�
		return -1;
	self->innerSend(&f);	                    ///<����pipe�ܵ���õ�msg��ת����_streamSock
	return 0;
}

/**
 * @brief ��Ϣת��
 * @details �Ѵ�pipe(���û�����)������frame����_streamSock���͸�peer
 * @param[in,out] zframe_t** �û���������Ϣ֡���������Ϣ���Զ�����
 * @return 
 * @retval 
 */
void SuperSocketImpl::innerSend(zframe_t** f)
{
	SSError err = SS_ERR_NONE;
	if (_identity.empty())    ///��idΪ��֡����δ������Ч����
	{
		err = SS_ERR_NOT_CONNECTED;
	}
	else                     ///����������Ч���ӣ������ݵ�_streamSock
	{
		zmsg_t* msg = zmsg_new();
		zmsg_addmem(msg, _identity.data(), _identity.size());
		zmsg_append(msg, f);
		auto rc = zmsg_send(&msg, _streamSock);
		if (rc != 0)
			err = SS_ERR_SEND;
	}
	if (_sndHandler)
		_sndHandler(err, _userPointer);///<�������ݻص�
}
#include "stdafx.h"
#include "SuperSocketAPI.h"
#include "HandlePool.h"

unique_ptr<HandlePool> uniquePool;///����һ��unique_ptr ָ��HandlePool������أ���һ������uniquePool
SUPERSOCKET_API SSError SS_InitSSLib()
{
	zsys_init();                ///��ʼ��CZMQ zsys ���Բ�ʹ�� ������socket��actorʱ�Զ���ʼ��
	if (uniquePool == nullptr)  ///���unique_ptr�����Ƿ�Ϊ�� ���� if(!uniquePool)
	{
		uniquePool = make_unique<HandlePool>();	///<�����ظ�����
	}
	return SS_ERR_NONE;
}

SUPERSOCKET_API SSError SS_DeinitSSLib()
{
	uniquePool.reset(); ///ɾ����֮�󶨵� rawָ�� ��unique_ptr�����ÿ�
	zsys_shutdown();
	return SS_ERR_NONE;
}

SUPERSOCKET_API SSError SS_CreateSocket(int* fd, const char* peerAddr, const SSEventCallback* callbacks)
{
	if (uniquePool == nullptr)                     ///���uniquePoolΪ��                                         
		return SS_ERR_INIT;                        ///����ʼ������
	string addr(peerAddr);                         ///ip��ַ�ַ���addr����192.168.000����
	*fd = uniquePool->newHandle(addr, *callbacks); ///����һ���µľ������һ��fd���������þ����ʶһ��SuperSocket����
	return *fd > 0 ? SS_ERR_NONE : SS_ERR_CREATE;
}

SUPERSOCKET_API SSError SS_DestroySocket(int fd)
{
	if (uniquePool == nullptr)
		return SS_ERR_INIT;
	return uniquePool->closeHandle(fd);            ///�رվ��
}

SUPERSOCKET_API SSError SS_SendBlock(int fd, const DataBlock& block)
{
	if (uniquePool == nullptr)
		return SS_ERR_INIT;
	return uniquePool->send(fd, block);            ///ͨ���������SuperSocket����������
}

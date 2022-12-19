#include "stdafx.h"
#include "SuperSocketAPI.h"
#include "HandlePool.h"

unique_ptr<HandlePool> uniquePool;///创建一个unique_ptr 指向HandlePool（句柄池）的一个对象uniquePool
SUPERSOCKET_API SSError SS_InitSSLib()
{
	zsys_init();                ///初始化CZMQ zsys 可以不使用 当创建socket和actor时自动初始化
	if (uniquePool == nullptr)  ///检查unique_ptr对象是否为空 或者 if(!uniquePool)
	{
		uniquePool = make_unique<HandlePool>();	///<避免重复创建
	}
	return SS_ERR_NONE;
}

SUPERSOCKET_API SSError SS_DeinitSSLib()
{
	uniquePool.reset(); ///删除与之绑定的 raw指针 将unique_ptr对象置空
	zsys_shutdown();
	return SS_ERR_NONE;
}

SUPERSOCKET_API SSError SS_CreateSocket(int* fd, const char* peerAddr, const SSEventCallback* callbacks)
{
	if (uniquePool == nullptr)                     ///如果uniquePool为空                                         
		return SS_ERR_INIT;                        ///则库初始化错误
	string addr(peerAddr);                         ///ip地址字符串addr（“192.168.000”）
	*fd = uniquePool->newHandle(addr, *callbacks); ///创建一个新的句柄返回一个fd描述符，该句柄标识一个SuperSocket对象
	return *fd > 0 ? SS_ERR_NONE : SS_ERR_CREATE;
}

SUPERSOCKET_API SSError SS_DestroySocket(int fd)
{
	if (uniquePool == nullptr)
		return SS_ERR_INIT;
	return uniquePool->closeHandle(fd);            ///关闭句柄
}

SUPERSOCKET_API SSError SS_SendBlock(int fd, const DataBlock& block)
{
	if (uniquePool == nullptr)
		return SS_ERR_INIT;
	return uniquePool->send(fd, block);            ///通过句柄调用SuperSocket对象发送数据
}

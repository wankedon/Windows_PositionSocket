/**
 * @file HandlePool.h
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief 句柄资源池
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#pragma once

#include "SuperSocketAPI.h"
/**
 * @brief 句柄资源池
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
	 * @brief 创建新的连接
	 * @details
	 * @param[in] peerAddress 远端主机地址
	 * @param[in] callbacks 回调函数集合
	 * @return int 
	 * @retval 新句柄
	 */
	int newHandle(const std::string& peerAddress, const SSEventCallback& callbacks);
	/**
	 * @brief 发送数据
	 * @details
	 * @param[in] handle socket句柄
	 * @param[in] block 待发送的数据
	 * @return SSError
	 */
	SSError send(int handle, const DataBlock& block);
	/**
	 * @brief 关闭句柄
	 * @details
	 * @param[int] handle socket句柄
	 * @return SSError
	 */
	SSError closeHandle(int handle);
private:
	map<int, shared_ptr<SuperSocket>> _socketsInUse;	///<在用的socket  键值对map
	list<int> _spareHandles;		///<备用句柄
	const int MAX_HANDLE_COUNT = 256;///<最大资源数量
	const int MAGIC_NUM = 0xCE41;	///<句柄标识生成
};


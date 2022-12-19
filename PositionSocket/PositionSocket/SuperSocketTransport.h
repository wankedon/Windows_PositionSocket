/**
* @file SuperSocketTransport.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief SuperSocketTransport类所在头文件

* @author wangkedong

* @date 2020:06:04

* @note 封装SuperSocket库

* @version <1.0.0>

*/
#pragma once
/**
* @brief 调用SuperSocket库，通过回调函数接收数据，通过发送函数发送数据
* @details
*/
namespace ZBSYB_RADAR_SOCKET
{
	class SuperSocketTransport
	{
	public:
		SuperSocketTransport();
		~SuperSocketTransport();
	public:
		PS_Error CreateSocket(const char* peerAddress, SSEventCallback& callback);
		PS_Error Send(const DataBlock& block);
		void DestroySocket();
	private:
		int m_socketHandle;
	};
}
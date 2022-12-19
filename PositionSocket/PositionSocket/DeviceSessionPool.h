/**
* @file DeviceSessionPool.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief DeviceSessionPool类所在头文件

* @author wangkedong

* @date 2020:07:09

* @note 句柄池，管理DeviceSession实例，建立句柄与实例对应关系

* @version <1.0.0>

*/
#pragma once
namespace ZBSYB_RADAR_SOCKET
{
	class DeviceSession; ///<声明类以便应用
	/**
	* @brief 句柄池，管理DeviceSession实例
	* @details
	*/
	class DeviceSessionPool
	{
	public:
		DeviceSessionPool();
		~DeviceSessionPool();
	public:
		std::pair<int, PS_Error> newHandle(const char* peerAddress, const SessionHandler* handler);
		PS_Error closeHandle(int handle);
		shared_ptr<DeviceSession> getSession(int handle);
	private:
		map<int, shared_ptr<DeviceSession>> m_socketsInUse; ///<在用的DeviceSession
		list<int> m_Handles;		        ///句柄池
		const int MAX_HANDLE_COUNT = 64;	///句柄数量
		const int MAGIC_NUM = 0xCE41;	    ///句柄标识生成
	};
}
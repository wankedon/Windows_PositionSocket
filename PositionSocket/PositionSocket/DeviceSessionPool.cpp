#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "DeviceSessionPool.h"
#include "DeviceSession.h"
namespace ZBSYB_RADAR_SOCKET
{
	DeviceSessionPool::DeviceSessionPool()
	{
		for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
		{
			m_Handles.push_back(MAGIC_NUM + i);   ///构造函数中构建MAX_HANDLE_COUNT个备用句柄
		}
	}

	DeviceSessionPool::~DeviceSessionPool()
	{
	}

	/**
	* @brief 创建新的连接
	* @param[in] peerAddress 远端主机地址
	* @param[in] handler 回调函数集合
	* @return pair<int, PS_Error>
	*/
	std::pair<int, PS_Error> DeviceSessionPool::newHandle(const char* peerAddress, const SessionHandler* handler)
	{
		if (m_Handles.empty())
			return std::pair<int, PS_Error>{-1, PS_ERR_NO_HANDLE_RESOURCE};
		auto session = make_shared<DeviceSession>();
		PS_Error err = session->Connect(peerAddress, handler);
		if (err == PS_ERR_CONNECT)
			return std::pair<int, PS_Error>{-1, err};
		int handle = m_Handles.front();
		assert(m_socketsInUse.find(handle) == m_socketsInUse.end());
		m_socketsInUse[handle] = session;
		m_Handles.pop_front();
		return std::pair<int, PS_Error>{handle, PS_ERR_NONE};
	}

	/**
	* @brief 获得DeviceSession的一个实例
	* @param[in] handle PositionSocket句柄
	* @return shared_ptr<DeviceSession>
	* @retval DeviceSession实例的指针
	*/
	shared_ptr<DeviceSession> DeviceSessionPool::getSession(int handle)
	{
		if (m_socketsInUse.find(handle) == m_socketsInUse.end()) ///handle未被使用
			return nullptr;
		return m_socketsInUse[handle]; ///通过键handle找到 PositionCommandChannel对象
	}

	/**
	* @brief 关闭句柄
	* @details
	* @param[int] handle PositionSocket句柄
	* @return PS_Error
	*/
	PS_Error DeviceSessionPool::closeHandle(int handle)
	{
		if (m_socketsInUse.find(handle) == m_socketsInUse.end())
			return PS_ERR_INVALID_HANDLE;
		m_socketsInUse[handle]->release();
		m_socketsInUse.erase(handle);			///< 排除socket，自动清理
		m_Handles.push_back(handle);		    ///< 归还句柄号
		return PS_ERR_NONE;
	}
}
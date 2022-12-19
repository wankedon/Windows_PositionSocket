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
			m_Handles.push_back(MAGIC_NUM + i);   ///���캯���й���MAX_HANDLE_COUNT�����þ��
		}
	}

	DeviceSessionPool::~DeviceSessionPool()
	{
	}

	/**
	* @brief �����µ�����
	* @param[in] peerAddress Զ��������ַ
	* @param[in] handler �ص���������
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
	* @brief ���DeviceSession��һ��ʵ��
	* @param[in] handle PositionSocket���
	* @return shared_ptr<DeviceSession>
	* @retval DeviceSessionʵ����ָ��
	*/
	shared_ptr<DeviceSession> DeviceSessionPool::getSession(int handle)
	{
		if (m_socketsInUse.find(handle) == m_socketsInUse.end()) ///handleδ��ʹ��
			return nullptr;
		return m_socketsInUse[handle]; ///ͨ����handle�ҵ� PositionCommandChannel����
	}

	/**
	* @brief �رվ��
	* @details
	* @param[int] handle PositionSocket���
	* @return PS_Error
	*/
	PS_Error DeviceSessionPool::closeHandle(int handle)
	{
		if (m_socketsInUse.find(handle) == m_socketsInUse.end())
			return PS_ERR_INVALID_HANDLE;
		m_socketsInUse[handle]->release();
		m_socketsInUse.erase(handle);			///< �ų�socket���Զ�����
		m_Handles.push_back(handle);		    ///< �黹�����
		return PS_ERR_NONE;
	}
}
/**
* @file DeviceSessionPool.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief DeviceSessionPool������ͷ�ļ�

* @author wangkedong

* @date 2020:07:09

* @note ����أ�����DeviceSessionʵ�������������ʵ����Ӧ��ϵ

* @version <1.0.0>

*/
#pragma once
namespace ZBSYB_RADAR_SOCKET
{
	class DeviceSession; ///<�������Ա�Ӧ��
	/**
	* @brief ����أ�����DeviceSessionʵ��
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
		map<int, shared_ptr<DeviceSession>> m_socketsInUse; ///<���õ�DeviceSession
		list<int> m_Handles;		        ///�����
		const int MAX_HANDLE_COUNT = 64;	///�������
		const int MAGIC_NUM = 0xCE41;	    ///�����ʶ����
	};
}
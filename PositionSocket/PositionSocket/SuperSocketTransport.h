/**
* @file SuperSocketTransport.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief SuperSocketTransport������ͷ�ļ�

* @author wangkedong

* @date 2020:06:04

* @note ��װSuperSocket��

* @version <1.0.0>

*/
#pragma once
/**
* @brief ����SuperSocket�⣬ͨ���ص������������ݣ�ͨ�����ͺ�����������
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
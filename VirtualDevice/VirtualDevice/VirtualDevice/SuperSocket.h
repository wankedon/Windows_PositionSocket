/**
 * @file SuperSocket.h

 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
 * ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

 * @brief SuperSocket�����ڵ�ͷ�ļ�

 * @author yangqing

 * @date 2020:03:26

 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��

 * @version <1.0.0>

 */

#include <string>
#include "SuperSocketAPI.h"

class SuperSocketImpl; ///<�������࣬�Ա���SuperSocket��ʹ��

/**
 * @brief �����࣬�ø����һ��ʵ��������peer�����ӣ�����DataBlock���շ�
 * ����ί��_impl�ṩ����_implΪSuperSocket����ʵ��
 */
namespace SuperSocketLib
{
	class SUPERSOCKET_API SuperSocket
	{
	public:
		SuperSocket(const std::string& address, const EventHandler* handler, bool client);
		~SuperSocket();
	public:
		SSErrorCode send(const NetBlock* block, const NodeId* to);
		void close();
	private:
		SuperSocketImpl* _impl;	///<����ʵ��
	};
}

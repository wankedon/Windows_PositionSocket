/**
 * @file SuperSocket.h

 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发

 * @brief SuperSocket类所在的头文件

 * @author yangqing

 * @date 2020:03:26

 * @note 依赖vcpkg提供的头文件和动态库

 * @version <1.0.0>

 */

#include <string>
#include "SuperSocketAPI.h"

class SuperSocketImpl; ///<声明该类，以便在SuperSocket中使用

/**
 * @brief 导出类，用该类的一个实例建立与peer的连接，进行DataBlock的收发
 * 该类委托_impl提供服务，_impl为SuperSocket具体实现
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
		SuperSocketImpl* _impl;	///<具体实现
	};
}

/**
 * @file SuperSocketAPI.h
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief SuperSocket库基础定义，包含C API
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */

/**
 * @mainpage SuperSocket库
 * @section CAPI c style API 使用方法
 * @subsection c_step1 Step 1: 引用SuperSocketAPI.h和SuperSocket.h，使用SS_InitSSLib()初始化库
 * @subsection c_step2 Step 2: 设置ip地址和回调函数集合，调用SuperSocket构造函数，创建一个SuperSocket对象
 * @subsection c_step3 Step 3: 利用该对象发送数据，通过你的回调函数来接收数据
 *
 * @section CppAPI c++ style API使用方法
 * @subsection cpp_step1 Step 1: 引用SuperSocketAPI.h，使用SS_InitSSLib()初始化库
 * @subsection cpp_step2 Step 2: 设置ip地址和回调函数集合，调用SS_CreateSocket，获得一个fd描述符
 * @subsection cpp_step3 Step 3: 利用该fd发送数据，通过你的回调函数来接收数据
 * @subsection cpp_step4 Step 4: 不再传输数据时，调用SS_DestroySocket()关闭连接
 * ## 不再使用库时调用SS_DeinitSSLib()关闭库，释放资源
 */
#pragma once
#ifdef SUPERSOCKET_EXPORTS
#define SUPERSOCKET_API __declspec(dllexport)
#else
#define SUPERSOCKET_API __declspec(dllimport)
#endif

namespace SuperSocketLib
{

	/**
	 * @brief API返回值，错误码标识
	 *
	 */
	enum SSErrorCode
	{
		SS_ERR_NONE = 0,	///<无错误
		SS_ERR_INVALID_HANDLE = -1,	///< 无效的句柄，针对C API
		SS_ERR_NOT_CONNECTED = -2,	///< 尚未建立有效连接
		SS_ERR_HANDLE_CLOSED = -3,	///< 句柄关闭
		SS_ERR_SEND = -4,			///< 发送数据过程出现错误
		SS_ERR_NOT_IMPLEMENTED = -5,	///<API 未实现
		SS_ERR_INIT = -6,			///< 库初始化错误
		SS_ERR_CREATE = -7,			///< socket创建错误
	};

	/**
	* @brief 传输的数据块结构体
	*/
	struct NetBlock
	{
		size_t len;	///< 数据块长度
		void* addr;	///< 数据块地址
	};
	
	struct NodeId
	{
		bool operator==(const NodeId& id) const
		{
			return (this->idLength == id.idLength)
				&& (memcmp(id.id, this->id, this->idLength) == 0);
		}
		bool operator<(const NodeId& id) const
		{
			if (id.idLength != this->idLength)
				return this->idLength < id.idLength;
			else
				return (memcmp(this->id, id.id, this->idLength) < 0);
		}
		NodeId()
		{
			strcpy_s(id, "invalidNodeId");
			idLength = 0;
		}
		NodeId(const void* data, size_t length)
		{
			if (length + 1 < sizeof(id))
			{
				idLength = length;
				memcpy(id, data, length);
				id[idLength] = 0;
			}
			else
			{
				strcpy_s(id, "invalidNodeId");
				idLength = 0;
			}
		}
		size_t idLength;
		char id[128];
	};

	typedef void SS_CONNECT_CALLBACK(const NodeId* id, void* userPointer);
	typedef void SS_DISCONNECT_CALLBACK(const NodeId* id, void* userPointer);
	typedef void SS_SEND_CALLBACK(SSErrorCode err, const NodeId* to, void* userPointer);
	typedef void SS_RECEIVE_CALLBACK(const NetBlock* data, const NodeId* from, void* userPointer);

	/**
	* @brief 回调函数集合
	*
	*/
	struct EventHandler
	{
		void* userPointer;
		SS_CONNECT_CALLBACK* cbConnect;	///<连接回调，当socket建立起连接时调用
		SS_DISCONNECT_CALLBACK* cbDisconnect;	///<断开回调，当socket断开时调用
		SS_SEND_CALLBACK* cbSend;	///<发送数据回调，当库把客户的数据块送到系统缓冲区时调用
		SS_RECEIVE_CALLBACK* cbReceive;	///<接收数据回调，当socket接收到数据时调用，客户从回调函数的DataBlock获取数据
	};

	/**
	 * @brief 初始化库 使用库时，第一个调用的函数，仅调用一次
	 */
	SUPERSOCKET_API SSErrorCode SS_InitSSLib();

	/**
	 * @brief 关闭库，最后一个调用的函数，仅调用一次
	 * @details 调用此函数之后，除了SS_InitSSLib(),不能再调用其他C接口函数
	 */
	SUPERSOCKET_API SSErrorCode SS_DeinitSSLib();


	SUPERSOCKET_API SSErrorCode SS_CreateServerSocket(int* fd, const char* listenAddr, const EventHandler* handler);
	/**
	 * @brief 创建一个socket，建立一个连接
	 * @param[out] fd 该函数返回时，将socket句柄填入该参数
	 * @param[in] peerAddr 服务器地址IP和端口字符串，例如"192.168.1.33:8080"
	 * @param[in] callbacks 回调函数集合，回调函数由客户开发，对应各种网络事件的响应函数
	 * @return SSError 错误码
	 * @retval SS_ERR_NONE or SS_ERR_CREATE
	 */
	SUPERSOCKET_API SSErrorCode SS_CreateClientSocket(int* fd, const char* peerAddr, const EventHandler* handler);

	/**
	 * @brief 销毁一个socket
	 * @param[int] fd socket句柄
	 * @return SSError 错误码
	 * @retval SS_ERR_NONE or SS_ERR_INVALID_HANDLE
	 */
	SUPERSOCKET_API SSErrorCode SS_DestroySocket(int fd);

	/**
	 * @brief 发送数据
	 * @detail 客户代码负责将待发送的数据填入一个DataBlock，然后将其传入该函数，由库负责发送给远端
	 * @param[in] fd socket句柄
	 * @param[in] block 待发送的数据块
	 * @return SSError 错误码
	 * @retval SS_ERR_NONE or SS_ERR_INVALID_HANDLE or SS_ERR_SEND
	 */
	SUPERSOCKET_API SSErrorCode SS_SendBlock(int fd, const NetBlock* data, const NodeId* to);
}


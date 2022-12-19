/**
 * @file Actor.cpp
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部
 * 版权所有，未经许可，不得复制、公开、转发
 
 * @brief actor类实现
 
 * @author yangqing
 
 * @date 2020:03:26
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#include "StdAfx.h"
#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor(void)
{
	
}

/**
 * @brief 静态函数作为pipe的后台线程函数，封装actorFunc
 * @detail
 * @param[in] pipe actor的pipe socket
 * @param[in] args this 指针 
 * @return 
 * @retval 
 */
void Actor::actorFuncWrap(zsock_t *pipe, void *args)
{
	Actor* self = (Actor*)args;
	self->actorFunc(pipe);
}

/**
 * @brief 纯虚函数
 * @details 基类中实现了一个基本操作，即给后台线程发启动信号
 * @param[int] pipe 管线socket
 * @return 
 * @retval 
 */
void Actor::actorFunc(zsock_t *pipe)
{
	zsock_signal(pipe, 0);
}

/**
 * @brief 发送数据
 * @details
 * @param[int] msg 待发送的消息
 * @param[out] 
 * @return 
 * @retval 
 */
int Actor::sendMsg(zmsg_t** msg)
{
	return zmsg_send(msg, actor);
}


/**
 * @brief 接收数据
 * @details 暂用不到
 * @param[int] 
 * @param[out] 
 * @return zmsg_t*
 * @retval 接收到的消息
 */
zmsg_t* Actor::receiveMsg()
{
	return zmsg_recv(actor);
}

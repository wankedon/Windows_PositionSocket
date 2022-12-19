/**
 * @file Actor.h
 
 * @brief Actor基类所处的头文件
 
 * @author yangqing
 
 * @par Copyright (c):中电科第四十一研究所 装备事业部 All Rights Reserved
 
 * @date 2020:03:25
 
 * @note 依赖vcpkg提供的头文件和动态库
 
 * @version <1.0.0>
 
 */
#pragma once

/**
 * @brief Actor基类，利用czmq的zactor
 * @details 原理：利用zeromq的pair模式实现前后台分离，利用管线pipe连接前后台，后台运行一线程，前台负责与外界交互
 */
class Actor
{
public:
	Actor();
	virtual ~Actor(void);
public:
	int sendMsg(zmsg_t** msg);
	zmsg_t* receiveMsg();
	static void actorFuncWrap(zsock_t *pipe, void *args); ///<提供统一线程函数模板
protected:
	virtual void actorFunc(zsock_t *pipe) = 0;	///<在各子类中自行实现
protected:
	zactor_t* actor;
};

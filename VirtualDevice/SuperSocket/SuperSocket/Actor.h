/**
 * @file Actor.h
 
 * @brief Actor����������ͷ�ļ�
 
 * @author yangqing
 
 * @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ�� All Rights Reserved
 
 * @date 2020:03:25
 
 * @note ����vcpkg�ṩ��ͷ�ļ��Ͷ�̬��
 
 * @version <1.0.0>
 
 */
#pragma once

/**
 * @brief Actor���࣬����czmq��zactor
 * @details ԭ������zeromq��pairģʽʵ��ǰ��̨���룬���ù���pipe����ǰ��̨����̨����һ�̣߳�ǰ̨��������罻��
 */
class Actor
{
public:
	Actor();
	virtual ~Actor(void);
public:
	int sendMsg(zmsg_t** msg);
	zmsg_t* receiveMsg();
	static void actorFuncWrap(zsock_t *pipe, void *args); ///<�ṩͳһ�̺߳���ģ��
protected:
	virtual void actorFunc(zsock_t *pipe) = 0;	///<�ڸ�����������ʵ��
protected:
	zactor_t* actor;
};

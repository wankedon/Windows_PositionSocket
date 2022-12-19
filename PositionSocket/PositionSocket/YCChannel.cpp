#include "stdafx.h"
#include "YCChannel.h"
namespace ZBSYB_RADAR_SOCKET
{
#define CLIENT_LIVENESS 10

	YCChannel::YCChannel()
		:DeviceChannel({ 'Y', 'C','\0' }),
		timerId(-1),
		clientLiveness(CLIENT_LIVENESS)
	{
		m_buffer = make_unique<DeviceStreamBuffer>();
		actor = zactor_new(actorFuncWrap, this);
	}

	YCChannel::~YCChannel()
	{
		stopTimer();
		///m_transport.reset();///20201026 wkd
	}

	PS_Error YCChannel::CreateConnect(const char* peerAddress, const YCCallbacks& yccallbacks, void* userPointer)
	{
		PS_Error erro = DeviceChannel::CreateConnect(peerAddress, yccallbacks.cbCon, userPointer);
		m_ycResultHandler = std::bind(yccallbacks.cbResult, std::placeholders::_1, userPointer);
		return erro;
	}

	void YCChannel::SS_OnReceiveComplete(const DataBlock& db)
	{
		clientLiveness = CLIENT_LIVENESS;//20201124wkd
		auto result = m_buffer->onDeviceData(db);
		for (auto frame : result)
		{
			YCResult ycResult = frame->makeYCResult();
			if (m_ycResultHandler)
			{
				m_ycResultHandler(&ycResult);
			}
		}
	}

	void YCChannel::stopTimer()
	{
		if (timerId > 0)
		{
			zloop_timer_end(zloop, timerId);
			timerId = -1;
		}
		if (actor)
		{
			zactor_destroy(&actor);
		}
	}

	void YCChannel::actorFuncWrap(zsock_t* pipe, void* args)
	{
		YCChannel* self = (YCChannel*)args;
		self->actorFunc(pipe); /// 基类实现actorFunc的通用部分 其它子类自行实现
	}

	/**
	* @brief 线程函数调用的成员函数，由各派生类具体实现，此处只实现actorFunc的通用部分
	*
	* @param pipe
	*/
	void YCChannel::actorFunc(zsock_t* pipe)
	{
		zsock_signal(pipe, 0);///基类处理：向前台发送信号，指明后台线程已经开始执行，'
		zloop = zloop_new();
		zloop_reader(zloop, pipe, pipeEventHandler, this); ///pipeEventHandler 父类实现
		startTimer(500, 0);
		zloop_start(zloop);
		zloop_destroy(&zloop);
	}

	int YCChannel::pipeEventHandler(zloop_t* loop, zsock_t* handle, void* arg)
	{
		static const char* TERM = "$TERM";
		zframe_t* f = zframe_recv(handle);          ///后台从pipe管道接收数据
		if (memcmp(TERM, zframe_data(f), 5) == 0)	///<退出的信息标识，返回-1，zloop将退出 memcmp比较
			return -1;
		else
			return 0;
	}

	//20201124wkd
	void YCChannel::startTimer(size_t interval, size_t times)
	{
		if (timerId != -1)
			return;
		timerId = zloop_timer(zloop, interval, times, timeHandler, this);
	}

	//20201124wkd
	int YCChannel::timeHandler(zloop_t* loop, int timerId, void* arg)
	{
		auto* self = (YCChannel*)arg;
		self->ontimer();
		return 0;
	}

	//20201124wkd
	void YCChannel::ontimer()
	{
		if (--clientLiveness == 0)
		{
			clientLiveness = CLIENT_LIVENESS;
			SS_OnAlreadyDisConnect();//20201124wkd
		}
	}
}
// VirtualDevice.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "precompile.h"
#include "VirtualDevice.h"
#include "ReplyFrame.h"

VirtualDevice::VirtualDevice()
	:zloop(nullptr),
	serverHandle(0)
{
}

VirtualDevice::~VirtualDevice()
{
	if (serverHandle != 0)
	{
		SS_DestroySocket(serverHandle);
		serverHandle = 0;
	}
}

int VirtualDevice::timeHandler(zloop_t* loop, int timerId, void* arg)
{
	auto* self = (VirtualDevice*)arg;
	self->_timer();
	return 0;
}

void VirtualDevice::onConnect(const NodeId* id, void* userPointer)
{
	auto* self = (VirtualDevice*)userPointer;
	self->_connect(id);
}

void VirtualDevice::onDisconnect(const NodeId* id, void* userPointer)
{
	auto* self = (VirtualDevice*)userPointer;
	self->_disconnect(id);
}

void VirtualDevice::onSend(SSErrorCode err, const NodeId* to, void* userPointer)
{
	auto* self = (VirtualDevice*)userPointer;
	self->_send(err, to);
}

void VirtualDevice::onReceive(const NetBlock* data, const NodeId* from, void* userPointer)
{
	auto* self = (VirtualDevice*)userPointer;
	self->_receive(data, from);
}

void VirtualDevice::_connect(const NodeId* id)
{
	if (clients.find(*id) != clients.end())
		zsys_debug("node already connected");
	clients[*id] = make_shared<Responser>();
}

void VirtualDevice::_disconnect(const NodeId* id)
{
	if (clients.find(*id) == clients.end())
		zsys_debug("node already disconnected");
	clients.erase(*id);
}

void VirtualDevice::_send(SSErrorCode err, const NodeId* to)
{
	//zsys_debug("sended");
}

void VirtualDevice::_receive(const NetBlock* data, const NodeId* from)
{
	if (clients.find(*from) == clients.end())
		zsys_debug("can't find this client");
	auto reply = clients[*from]->onRequest(data);
	if (reply != nullptr)
	{
		NetBlock b(reply->MakeBlock());
		SS_SendBlock(serverHandle, &b, from);
	}
}

void VirtualDevice::_timer()
{
	for (auto client : clients)
	{
		auto measureResult = client.second->onTimer();
		if (measureResult != nullptr)
		{
			NetBlock b(measureResult->MakeBlock());
			SS_SendBlock(serverHandle, &b, &client.first);
		}
	}
}

void VirtualDevice::start(const string& addr)
{
	EventHandler handler{ 0 };
	handler.cbConnect = onConnect;
	handler.cbDisconnect = onDisconnect;
	handler.cbReceive = onReceive;
	handler.cbSend = onSend;
	handler.userPointer = this;
	SSErrorCode error = SS_CreateServerSocket(&serverHandle, addr.c_str(), &handler);
	if (error != SS_ERR_NONE)
		return;
	zloop = zloop_new();
	zloop_timer(zloop, 100, 0, timeHandler, this);
	zloop_start(zloop);
	//循环结束后清理
	zloop_destroy(&zloop);
}

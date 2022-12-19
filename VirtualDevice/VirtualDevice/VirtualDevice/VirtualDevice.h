#pragma once

#include "Responser.h"
class VirtualDevice
{
public:
	VirtualDevice();
	~VirtualDevice();
public:
	void start(const string& addr);
private:
	static void onConnect(const NodeId* id, void* userPointer);
	static void onDisconnect(const NodeId* id, void* userPointer);
	static void onSend(SSErrorCode err, const NodeId* to, void* userPointer);
	static void onReceive(const NetBlock* data, const NodeId* from, void* userPointer);
	static int timeHandler(zloop_t* loop, int timerId, void* arg);
	void _connect(const NodeId* id);
	void _disconnect(const NodeId* id);
	void _send(SSErrorCode err, const NodeId* to);
	void _receive(const NetBlock* data, const NodeId* from);
	void _timer();
private:
	zloop_t* zloop;
	int serverHandle;
	std::map<SuperSocketLib::NodeId, shared_ptr<Responser>> clients;
};


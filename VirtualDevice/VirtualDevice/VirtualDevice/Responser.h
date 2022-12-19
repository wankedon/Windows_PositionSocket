#pragma once

#include "CmdProcessor.h"
#include "DataProcessor.h"
#include "SuperSocketAPI.h"
#include "ReplyFrame.h"

using namespace SuperSocketLib;
class Responser
{
public:
	Responser();
	~Responser();
public:
	shared_ptr<ReplyFrame> onRequest(const NetBlock* data);
	shared_ptr<ReplyFrame> onTimer();
private:
	shared_ptr<ReplyFrame> parse(const NetBlock* data);
private:
	string type;
	vector<byte> buffer;
	shared_ptr<ReplyFrame> lastMeasurementFrame;
	uint32_t scanIdx;
};


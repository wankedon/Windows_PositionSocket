
#include "precompile.h"
#include "Responser.h"

Responser::Responser()
	:scanIdx(0)
{
}

Responser::~Responser()
{
}

shared_ptr<ReplyFrame> Responser::onRequest(const NetBlock* data)
{
	shared_ptr<ReplyFrame> result(parse(data));
	if (result != nullptr)
	{
		result->print();
		auto id = result->id();
		//一些特殊处理
		if (id == START_STOP_ID || id == LOCK_TRACK_ID)
		{
			lastMeasurementFrame = make_shared<ReplyFrame>(*result);
			result = nullptr;
			scanIdx = 0;
		}
		else if (id == SELF_DETECT_ID)//自检指令则停止回传结果
		{
			lastMeasurementFrame = nullptr;
			buffer.clear();
			scanIdx = 0;
		}
	}
	return result;
}

shared_ptr<ReplyFrame> Responser::onTimer()	//模拟一次扫描
{
	if (lastMeasurementFrame != nullptr)
	{
		lastMeasurementFrame->randomGene(scanIdx);
		scanIdx++;
	}
	return lastMeasurementFrame;
}

shared_ptr<ReplyFrame>  Responser::parse(const NetBlock* data)
{
	if (type.empty())
	{
		assert(data->len == 3);
		type = (char*)(data->addr);
		return nullptr;
	}
	vector<byte> temp{ (byte*)data->addr, (byte*)data->addr + data->len };
	if (buffer.empty())
	{
		auto result = make_shared<ReplyFrame>(temp);
		if (result->size() == 0)
		{
			buffer = temp;	//暂存结果
			return nullptr;
		}
		else
		{
			return result;
		}
	}
	else
	{
		buffer.insert(buffer.end(), temp.begin(), temp.end());
		auto result = make_shared<ReplyFrame>(buffer);
		if (result->size() == 0)
		{
			return nullptr;
		}
		else
		{
			buffer.clear();	//清缓冲
			return result;
		}
	}
}


/**
* @file DeviceStreamBuffer.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

/**
* @file DeviceStreamBuffer.h
* @brief 字节流缓冲区
* @author 装备事业部软件组 王克东
* @date 2021:07:05
* @Copyright (c) 2021 中国电子科技集团公司第四十一研究所
*/ 
#pragma once
#include "DeviceFrame.h"
namespace ZBSYB_RADAR_SOCKET
{
	class DeviceStreamBuffer
	{
	public:
		DeviceStreamBuffer();
		~DeviceStreamBuffer();
	public:
		list<shared_ptr<DeviceFrame>> onDeviceData(const DataBlock& db);
	private:
		list<shared_ptr<vector<uint32_t>>> extractFullFrame(vector<byte>& buffer);
		vector<int> extractFrameHead(vector<uint32_t>& allFrame, vector<byte>& byteBuffer);
		void multiFrameExtract(vector<uint32_t>& allFrame, list<shared_ptr<vector<uint32_t>>>& frameList);
		void singleFrameExtract(vector<uint32_t>& allFrame, list<shared_ptr<vector<uint32_t>>>& frameList);
	private:
		vector<byte> m_bytebuffer;
		vector<int> m_headIndex;
	private:
		static const int OTHER_LENGTH = 3;
	};
}
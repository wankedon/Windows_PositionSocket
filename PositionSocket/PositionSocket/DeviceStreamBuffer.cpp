/**
* @file DeviceStreamBuffer.cpp
* @brief 字节流缓冲区
* @author 装备事业部软件组 王克东
* @date 2021:07:05
* @Copyright (c) 2021 中国电子科技集团公司第四十一研究所
*/ 
#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "DeviceStreamBuffer.h"
namespace ZBSYB_RADAR_SOCKET
{
	DeviceStreamBuffer::DeviceStreamBuffer()
	{
	}

	DeviceStreamBuffer::~DeviceStreamBuffer()
	{
	}

	/**
	* @brief 数据块放入缓冲，提取完整数据帧
	* @param[in] db 接收数据块
	*/
	list<shared_ptr<DeviceFrame>> DeviceStreamBuffer::onDeviceData(const DataBlock& db)
	{
		list<shared_ptr<DeviceFrame>> result;
		vector<byte> temp{ (byte*)db.addr, (byte*)db.addr + db.len };
		m_bytebuffer.insert(m_bytebuffer.end(), temp.begin(), temp.end());
		while (true)
		{
			if (m_bytebuffer.size() == 0)
				break;
			auto frameList = extractFullFrame(m_bytebuffer);//+++
			if (frameList.size() == 0)
				break;
			for (auto fr : frameList)//+++
			{
				auto ptr = *fr.get();//+++
				vector<byte> fb(ptr.size() * sizeof(uint32_t));//+++
				memcpy(fb.data(), ptr.data(), ptr.size() * sizeof(uint32_t));//+++
				auto frame = make_shared<DeviceFrame>(fb);//(m_bytebuffer);
				if (frame->size() == 0)
					break;
				result.push_back(frame);
				m_bytebuffer.erase(m_bytebuffer.begin(), m_bytebuffer.begin() + frame->size() * sizeof(uint32_t));
			}//+++
		}
		return result;
	}

	/**
	* @brief 提取完整帧
	* @param[in] buffer 字节数组
	*/
	list<shared_ptr<vector<uint32_t>>> DeviceStreamBuffer::extractFullFrame(vector<byte>& byteBuffer)
	{
		list<shared_ptr<vector<uint32_t>>> frameList;
		if (byteBuffer.size() < 2 * sizeof(uint32_t))
			return frameList;
		vector<uint32_t> allFrame;
		m_headIndex = extractFrameHead(allFrame, byteBuffer);
		if (m_headIndex.size() == 0)
			return frameList;
		if (m_headIndex.size() > 1)   //包含多个帧头
		{
			multiFrameExtract(allFrame, frameList);
		}
		else//包含一个帧头
		{
			allFrame = { allFrame.begin() + m_headIndex[0],allFrame.end() };
		}
		//多帧剩余数据或包含单帧头
		singleFrameExtract(allFrame, frameList);
		return frameList;
	}

	/**
	* @brief 提取帧头
	* @param[out] allFrame uint32_t数组
	* @param[in]  byteBuffer 字节数组
	* @return vector<int> 帧头位置
	*/
	vector<int> DeviceStreamBuffer::extractFrameHead(vector<uint32_t>& allFrame, vector<byte>& byteBuffer)
	{
		allFrame.resize(byteBuffer.size() / sizeof(uint32_t));
		memcpy(allFrame.data(), byteBuffer.data(), allFrame.size() * sizeof(uint32_t));
		vector<int> headIndex(0);//帧头位置
		int index = 0;
		for (auto fr : allFrame)
		{
			(fr == SYNC_WORD_ZK || fr == SYNC_WORD_YC) ? headIndex.emplace_back(index++) : index++;
		}
		return headIndex;
	}

	/**
	* @brief 多帧提取
	* @param[out] allFrame 提取帧后剩余uint32_t数组
	* @param[out]  frameList 提取的完整帧
	*/
	void DeviceStreamBuffer::multiFrameExtract(vector<uint32_t>& allFrame, list<shared_ptr<vector<uint32_t>>>& frameList)
	{
		if (m_headIndex.size() == 0)
			return;
		int eraseSize = 0;//擦除的四字节数
		for (int i = 0; i < m_headIndex.size() - 1; i++)
		{
			vector<uint32_t> oneFrame = { allFrame.begin() + m_headIndex[i],allFrame.begin() + m_headIndex[i + 1] };
			frameList.push_back(make_shared<vector<uint32_t>>(oneFrame));
			eraseSize += allFrame[m_headIndex[i] + 1] / sizeof(uint32_t) + OTHER_LENGTH;
		}
		//擦除完整帧
		allFrame.erase(allFrame.begin() + m_headIndex[0], allFrame.begin() + eraseSize);
	}

	/**
	* @brief 单帧提取
	* @param[out] allFrame 提取帧后剩余uint32_t数组
	* @param[out]  frameList 提取的完整帧
	*/
	void DeviceStreamBuffer::singleFrameExtract(vector<uint32_t>& allFrame, list<shared_ptr<vector<uint32_t>>>& frameList)
	{
		if (allFrame.size() < 2)
			return;
		int lastFrameSize = allFrame[1] / sizeof(uint32_t) + OTHER_LENGTH;
		if (allFrame.size() < lastFrameSize)
			return;
		vector<uint32_t> oneFrame = { allFrame.begin(),allFrame.begin() + lastFrameSize };
		frameList.push_back(make_shared<vector<uint32_t>>(oneFrame));
		//擦除
		allFrame.erase(allFrame.begin(), allFrame.begin() + lastFrameSize);
	}
}
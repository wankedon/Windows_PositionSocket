/**
* @file DeviceFrame.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief DeviceFrame类所在头文件

* @author wangkedong

* @date 2020:07:09

* @note 从字节流中提取完整数据帧

* @version <1.0.0>

*/
#pragma once
#include "positionSocketAPI.h"
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief 从字节流中提取完整数据帧
	* @details
	*/
	class DeviceFrame
	{
	public:
		DeviceFrame(const vector<byte>& deviceStream);
		~DeviceFrame();
	public:
		bool isCmdReply() const { return size() == REPLY_FRAME_SIZE; }
		size_t size() const;
		uint32_t id() const;
		uint32_t index() const;
		ZKResult makeZKResult();
		YCResult makeYCResult();
		template<class T>
		bool makeCmdResult(T& result)
		{
			if (m_data == nullptr)
				return false;
			if (size() * sizeof(uint32_t) != sizeof(result))
				return false;
			memcpy(&result, m_data->data(), sizeof(result));
			return true;
		}
	private:
		unique_ptr<vector<uint32_t>> m_data;
	private:
		static const size_t FIXED_LENGTH = 12;
		static const size_t REPLY_FRAME_SIZE = 32;
	};
}
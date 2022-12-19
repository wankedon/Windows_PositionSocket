/**
* @file CmdResponseConverter.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief CmdResponseConverter������ͷ�ļ�

* @author wangkedong

* @date 2020:07:09

* @note 

* @version <1.0.0>

*/

#pragma once
#include "DeviceFrame.h"
#include "PositionSocketAPI.h"
#include <set>
namespace ZBSYB_RADAR_SOCKET
{

	/**
	* @brief ת��������Ӧ���
	* @details
	*/
	class CmdResponseConverter
	{
	public:
		CmdResponseConverter() {}
		~CmdResponseConverter() {}
	public:
		template<class T>
		bool convertToResult(T& result)
		{
			if (m_currentFrame == nullptr)
				return false;
			if (!m_currentFrame->isCmdReply())
				return false;
			return m_currentFrame->makeCmdResult<T>(result);
		}
		bool detectReply(uint32_t channelCmdSequenceNum) const
		{
			if (m_currentFrame == nullptr)
				return false;
			return (m_currentFrame->index() == channelCmdSequenceNum);
		}
		static bool isCmdFrame(uint32_t id)
		{
			return (CMD_NAME.find(id) != CMD_NAME.end());
		}
		static bool isStreamFrame(uint32_t id)
		{
			return (STREAM_NAME.find(id) != STREAM_NAME.end());
		}
		void setCurrentFrame(shared_ptr<DeviceFrame> frame)
		{
			m_currentFrame = frame;
		}
	private:
		shared_ptr<DeviceFrame> m_currentFrame;
	private:
		static const set<uint32_t> CMD_NAME;
		static const set<uint32_t> STREAM_NAME;
	};
}
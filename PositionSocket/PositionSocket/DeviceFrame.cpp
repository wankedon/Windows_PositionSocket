#include "stdafx.h"
#include "DeviceFrame.h"
namespace ZBSYB_RADAR_SOCKET
{
	DeviceFrame::DeviceFrame(const vector<byte>& deviceStream)
	{
		if (deviceStream.size() < REPLY_FRAME_SIZE * 4)
			return;
		uint32_t expectedLength;
		memcpy(&expectedLength, &deviceStream[4], sizeof(uint32_t));
		expectedLength += FIXED_LENGTH;
		if (deviceStream.size() < expectedLength)
			return;
		m_data = make_unique<vector<uint32_t>>(expectedLength / sizeof(uint32_t));
		memcpy(m_data->data(), deviceStream.data(), expectedLength);
	}

	DeviceFrame::~DeviceFrame()
	{
	}

	/**
	* @brief ��ȡ����֡������ID
	* @return ����֡����ID
	*/
	uint32_t DeviceFrame::id() const
	{
		if (m_data == nullptr)
			return 0;
		else
			return (*m_data)[2];
	}

	/**
	* @brief ��ȡ����֡����Ӧ����
	* @return ��Ӧ����
	*/
	uint32_t DeviceFrame::index() const
	{
		if (m_data == nullptr)
			return 0;
		else
			return (*m_data)[4];
	}

	/**
	* @brief ��ȡ����֡��С
	* @return ��Ӧ����
	*/
	size_t DeviceFrame::size() const
	{
		if (m_data == nullptr)
			return 0;
		else
			return m_data->size();
	}

	/**
	* @brief ��ȡ�տս��
	* @return ZKResult
	*/
	ZKResult DeviceFrame::makeZKResult()
	{
		if (m_data == nullptr)
			return ZKResult{ nullptr, nullptr };
		else if (id() == SELF_DETECT_ID)
			return ZKResult{ (SelfDetectFrame*)m_data->data(), nullptr ,nullptr };
		else if (id() == START_STOP_ID)
			return ZKResult{ nullptr, (StartStopFrame*)m_data->data(), nullptr };
		else if (id() == LOCK_TRACK_ID)
			return ZKResult{ nullptr,nullptr,(LockTrackFrame*)m_data->data() };
		else
			return ZKResult{ nullptr, nullptr,nullptr };
	}

	/**
	* @brief ��ȡң����
	* @return YCResult
	*/
	YCResult DeviceFrame::makeYCResult()
	{
		if (m_data == nullptr)
			return YCResult{ 0, nullptr };
		else
			return YCResult{ m_data->size(), m_data->data() };
	}
}
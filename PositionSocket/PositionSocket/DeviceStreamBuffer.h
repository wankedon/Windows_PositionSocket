/**
* @file DeviceStreamBuffer.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

/**
* @file DeviceStreamBuffer.h
* @brief �ֽ���������
* @author װ����ҵ������� ���˶�
* @date 2021:07:05
* @Copyright (c) 2021 �й����ӿƼ����Ź�˾����ʮһ�о���
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
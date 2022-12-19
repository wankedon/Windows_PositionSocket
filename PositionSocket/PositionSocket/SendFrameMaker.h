/**
* @file SendFrameMaker.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief SendFrameMaker������ͷ�ļ�

* @author wangkedong

* @date 2020:06:04

* @note ������������֡

* @version <1.0.0>

*/
#pragma once
/**
* @brief ������������֡
* @details
*/
namespace ZBSYB_RADAR_SOCKET
{
	class SendFrameMaker
	{
	public:
		SendFrameMaker();
		~SendFrameMaker();
	public:
		DataBlock makeSelfDetectCmdFrame();
		DataBlock makeStartStopCmdFrame(uint32_t option);
		DataBlock makeLockTrackCmdFrame(const LockTrackParam& locktrack);
		DataBlock makeResetCmdFrame();
		DataBlock makeParaConfCmdFrame(const ParamConfParam& paramconf);
		DataBlock makeGPSModConfCmdFrame();
		DataBlock makeCompassCalibCmdFrame();
		DataBlock makeIPConfCmdFrame(uint32_t address);
		DataBlock makeLowPowerCmdFrame(uint32_t option);
		DataBlock makeWaveGateCmdFrame(uint32_t option);
		DataBlock makeAGCCtrlCmdFrame(uint32_t option, uint32_t value);
		DataBlock makeDetectThrdCmdFrame(uint32_t value);
		DataBlock makeSoftwareUpdateCmdFrame(uint32_t option, uint32_t type);
		DataBlock makeDeviceStatusSearchCmdFrame();
		DataBlock makeStandardCalibCmdFrame(uint32_t frqvalue);
		DataBlock makeTimeCalibCmdFrame(uint32_t ntpAddress);
		uint32_t getsequenceNum();
	private:
		DataBlock makeSendCmdFrame(uint32_t cmdId);
		uint32_t calculateChecksum();
	private:
		DataBlock m_block;
		uint32_t m_sequenceNum;
		vector<uint32_t> m_senddata;
	private:
		static const size_t CMD_FRAME_SIZE = 32;
	};
}
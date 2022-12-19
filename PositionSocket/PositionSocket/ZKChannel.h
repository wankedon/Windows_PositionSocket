/**
* @file ZKChannel.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief ZKChannel类所在头文件

* @author wangkedong

* @date 2020:06:04

* @note 综控通路ZKChannel继承自DeviceChannel

* @version <1.0.0>

*/
#pragma once
#include "DeviceChannel.h"
#include "CmdResponseConverter.h"
#include "SendFrameMaker.h"
#include <chrono> 
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief 综控通路ZKChannel继承自DeviceChannel
	* @details
	*/
	class ZKChannel :
		public DeviceChannel
	{
	public:
		ZKChannel();
		virtual ~ZKChannel();
	public:
		PS_Error CreateConnect(const char* peerAddress, const ZKCallbacks& zkcallbacks, void* userPointer);
		PS_Error SendSelfDetectCmd();
		PS_Error SendStartStopCmd(uint32_t option);
		PS_Error SendLockTrackCmd(const LockTrackParam* locktrack);
		PS_Error SendResetCmd(ResetFrame* resetframe);
		PS_Error SendParaConfCmd(ParamConfFrame* paramconfframe, const ParamConfParam* paramconf);
		PS_Error SendGPSModConfCmd(GPSConfFrame* gpsconfframe);
		PS_Error SendCompassCalibCmd(CompassCalFrame* compasscalframe);
		PS_Error SendIPConfCmd(IpConfFrame* ipconfframe, uint32_t address);
		PS_Error SendLowPowerCmd(LowPowerConFrame* lowpowerconframe, uint32_t option);
		PS_Error SendWaveGateCmd(WaveGateSwitchFrame* wavegateswitchframe, uint32_t option);
		PS_Error SendAGCCtrlCmd(AgcControlFrame* agccontrolframe, uint32_t option, uint32_t value);
		PS_Error SendDetectThrdCmd(DetectThresholdFrame* detectthresholdframe, uint32_t value);
		PS_Error SendSoftwareUpdateCmd(SoftwareUpdateFrame* softwareupdateframe, uint32_t option, uint32_t type);
		PS_Error SendDeviceStatusSearchCmd(DeviceStatusSearchFrame* devicestatussearchframe);
		PS_Error SendStandardCalibCmd(StandardCalibFrame* standardcalibframe, uint32_t frqvalue);
		PS_Error SendTimeCalib(TimeCalibFrame* timeCalibFrame,uint32_t ntpAddress);
	private:
		PS_Error sendFrame(const DataBlock& block);
		void initalResponseFlag();
		PS_Error wait(int flag);
		void notify();
		virtual void SS_OnReceiveComplete(const DataBlock& db);
	private:
		std::mutex m_readyMutex;
		unique_ptr<DeviceStreamBuffer> m_buffer;
		std::condition_variable m_readyCondVar;
		unique_ptr<SendFrameMaker> m_FrameMaker;
		unique_ptr<CmdResponseConverter> m_cmdConverter;
		function<void(const ZKResult* zkResult)> m_zkResultHandler;
	private:
		map<int, string> m_responseFlag;
		static const size_t CMD_FRAME_SIZE = 32;
		const chrono::seconds TIME_OUT_VALUE = 5s;
	};
}
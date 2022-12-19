/**
* @file DeviceSession.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief DeviceSession������ͷ�ļ�

* @author wangkedong

* @date 2020:07:09

* @note ��װZKChannel��YCChannel

* @version <1.0.0>

*/

#pragma once
#include "ZKChannel.h"
#include "YCChannel.h"
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief ��װZKChannel��YCChannel
	* @details
	*/
	class DeviceSession
	{
	public:
		DeviceSession();
		~DeviceSession();
	public:
		void release();
	public:
		PS_Error Connect(const char* peerAddress, const SessionHandler* handler);
		PS_Error SendSelfDetect();
		PS_Error SendStartStop(uint32_t option);
		PS_Error SendLockTrack(const LockTrackParam* locktrack);
		PS_Error SendReset(ResetFrame* resetframe);
		PS_Error SendParaConf(ParamConfFrame* paramconfframe, const ParamConfParam* paramconf);
		PS_Error SendGPSModConf(GPSConfFrame* gpsconfframe);
		PS_Error SendCompassCalib(CompassCalFrame* compasscalframe);
		PS_Error SendIPConf(IpConfFrame* ipconfframe, uint32_t address);
		PS_Error SendLowPower(LowPowerConFrame* lowpowerconframe, uint32_t option);
		PS_Error SendWaveGate(WaveGateSwitchFrame* wavegateswitchframe, uint32_t option);
		PS_Error SendAGCCtrl(AgcControlFrame* agccontrolframe, uint32_t option, uint32_t value);
		PS_Error SendDetectThrd(DetectThresholdFrame* detectthresholdframe, uint32_t value);
		PS_Error SendSoftwareUpdate(SoftwareUpdateFrame* softwareupdateframe, uint32_t option, uint32_t type);
		PS_Error DeviceStatusSearch(DeviceStatusSearchFrame* devicestatussearchframe);
		PS_Error SendStandardCalib(StandardCalibFrame* standardcalibframe, uint32_t frqvalue);
		PS_Error SendTimeCalib(TimeCalibFrame* timeCalibFrame,uint32_t ntpAddress);
	private:
		unique_ptr<ZKChannel> zkChannel;
		unique_ptr<YCChannel> ycChannel;
	};
}
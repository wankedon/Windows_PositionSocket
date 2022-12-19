#include "stdafx.h"
#include "DeviceSession.h"
namespace ZBSYB_RADAR_SOCKET
{
	DeviceSession::DeviceSession()
	{
		zkChannel = make_unique<ZKChannel>();
		ycChannel = make_unique<YCChannel>();
	}

	DeviceSession::~DeviceSession()
	{
	}

	void DeviceSession::release()
	{
		if (zkChannel)
		{
			zkChannel.reset();
		}
		if (ycChannel)
		{
			ycChannel.reset();
		}
	}

	/**
	* @brief �����µ�����
	* @details �����տغ�ң��ͨ·����
	* @param[in] peerAddress Զ��������ַ
	* @param[in] handler �ص���������
	* @return PS_Error
	*/
	PS_Error DeviceSession::Connect(const char* peerAddress, const SessionHandler* handler)
	{
		PS_Error errzk = zkChannel->CreateConnect(peerAddress, handler->cbZK, handler->userPointer);
		PS_Error erryc = ycChannel->CreateConnect(peerAddress, handler->cbYC, handler->userPointer);
		if (errzk == PS_ERR_NONE && erryc == PS_ERR_NONE)
		{
			return PS_ERR_NONE;
		}
		else
		{
			return PS_ERR_CONNECT;
		}
	}

	/**
	* @brief ����ָ��
	* @details ͨ���տ�ͨ·����ָ��
	* @param[out] selfdetectframe �Լ�ָ��ش����
	* @return PS_Error
	*/
	PS_Error DeviceSession::SendSelfDetect()
	{
		return zkChannel->SendSelfDetectCmd();
	}

	PS_Error DeviceSession::SendStartStop(uint32_t option)
	{
		return zkChannel->SendStartStopCmd(option);
	}

	PS_Error DeviceSession::SendLockTrack(const LockTrackParam* locktrack)
	{
		return zkChannel->SendLockTrackCmd(locktrack);
	}

	PS_Error DeviceSession::SendReset(ResetFrame* resetframe)
	{
		return zkChannel->SendResetCmd(resetframe);
	}

	PS_Error DeviceSession::SendParaConf(ParamConfFrame* paramconfframe, const ParamConfParam* paramconf)
	{
		return zkChannel->SendParaConfCmd(paramconfframe, paramconf);
	}

	PS_Error DeviceSession::SendGPSModConf(GPSConfFrame* gpsconfframe)
	{
		return zkChannel->SendGPSModConfCmd(gpsconfframe);
	}

	PS_Error DeviceSession::SendCompassCalib(CompassCalFrame* compasscalframe)
	{
		return zkChannel->SendCompassCalibCmd(compasscalframe);
	}

	PS_Error DeviceSession::SendIPConf(IpConfFrame* ipconfframe, uint32_t address)
	{
		return zkChannel->SendIPConfCmd(ipconfframe, address);
	}

	PS_Error DeviceSession::SendLowPower(LowPowerConFrame* lowpowerconframe, uint32_t option)
	{
		return zkChannel->SendLowPowerCmd(lowpowerconframe, option);
	}

	PS_Error DeviceSession::SendWaveGate(WaveGateSwitchFrame* wavegateswitchframe, uint32_t option)
	{
		return zkChannel->SendWaveGateCmd(wavegateswitchframe, option);
	}

	PS_Error DeviceSession::SendAGCCtrl(AgcControlFrame* agccontrolframe, uint32_t option, uint32_t value)
	{
		return zkChannel->SendAGCCtrlCmd(agccontrolframe, option, value);
	}

	PS_Error DeviceSession::SendDetectThrd(DetectThresholdFrame* detectthresholdframe, uint32_t value)
	{
		return zkChannel->SendDetectThrdCmd(detectthresholdframe, value);
	}

	PS_Error DeviceSession::SendSoftwareUpdate(SoftwareUpdateFrame* softwareupdateframe, uint32_t option, uint32_t type)
	{
		return zkChannel->SendSoftwareUpdateCmd(softwareupdateframe, option, type);
	}

	PS_Error DeviceSession::DeviceStatusSearch(DeviceStatusSearchFrame* devicestatussearchframe)
	{
		return zkChannel->SendDeviceStatusSearchCmd(devicestatussearchframe);
	}

	PS_Error DeviceSession::SendStandardCalib(StandardCalibFrame* standardcalibframe, uint32_t frqvalue)
	{
		return zkChannel->SendStandardCalibCmd(standardcalibframe, frqvalue);
	}

	PS_Error DeviceSession::SendTimeCalib(TimeCalibFrame* timeCalibFrame, uint32_t ntpAddress)
	{
		return zkChannel->SendTimeCalib(timeCalibFrame, ntpAddress);
	}

}
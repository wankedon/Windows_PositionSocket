#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "DeviceSession.h"
#include "DeviceSessionPool.h"

namespace ZBSYB_RADAR_SOCKET
{
	unique_ptr<DeviceSessionPool> sessiobPool;
	POSITIONSOCKET_API PS_Error PS_InitSSLib()
	{
		SS_InitSSLib();     ///调用SuperSocket库函数创建库         
		if (sessiobPool == nullptr)
		{
			sessiobPool = make_unique<DeviceSessionPool>();
		}
		return PS_ERR_NONE;
	}

	POSITIONSOCKET_API PS_Error PS_DeinitSSLib()
	{
		sessiobPool.reset();
		SS_DeinitSSLib();    ///调用SuperSocket库函数关闭库
		return PS_ERR_NONE;
	}

	POSITIONSOCKET_API PS_Error PS_CreateConnect(int* fd, const char* peerAddress, const SessionHandler* handler)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto pair = sessiobPool->newHandle(peerAddress, handler);
		*fd = pair.first;
		return pair.second;
	}

	POSITIONSOCKET_API PS_Error PS_SendSelfDetect(int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendSelfDetect();
	}

	POSITIONSOCKET_API PS_Error PS_SendStartStop(int fd, uint32_t option)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendStartStop(option);
	}

	POSITIONSOCKET_API PS_Error PS_SendLockTrack(int fd, const LockTrackParam* locktrack)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendLockTrack(locktrack);
	}

	POSITIONSOCKET_API PS_Error PS_SendReset(ResetFrame* resetframe, int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendReset(resetframe);
	}

	POSITIONSOCKET_API PS_Error PS_SendParaConf(ParamConfFrame* paramconfframe, int fd, const ParamConfParam* paramconf)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendParaConf(paramconfframe, paramconf);
	}

	POSITIONSOCKET_API PS_Error PS_SendGPSModConf(GPSConfFrame* gpsconfframe, int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendGPSModConf(gpsconfframe);
	}

	POSITIONSOCKET_API PS_Error PS_SendCompassCalib(CompassCalFrame* compasscalframe, int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendCompassCalib(compasscalframe);
	}

	POSITIONSOCKET_API PS_Error PS_SendIPConf(IpConfFrame* ipconfframe, int fd, uint32_t address)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendIPConf(ipconfframe, address);
	}

	POSITIONSOCKET_API PS_Error PS_SendLowPower(LowPowerConFrame* lowpowerconframe, int fd, uint32_t option)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendLowPower(lowpowerconframe, option);
	}

	POSITIONSOCKET_API PS_Error PS_SendWaveGate(WaveGateSwitchFrame* wavegateswitchframe, int fd, uint32_t option)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendWaveGate(wavegateswitchframe, option);
	}

	POSITIONSOCKET_API PS_Error PS_SendAGCCtrl(AgcControlFrame* agccontrolframe, int fd, uint32_t option, uint32_t agcvalue)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendAGCCtrl(agccontrolframe, option, agcvalue);
	}

	POSITIONSOCKET_API PS_Error PS_SendDetectThrd(DetectThresholdFrame* detectthresholdframe, int fd, uint32_t value)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendDetectThrd(detectthresholdframe, value);
	}

	POSITIONSOCKET_API PS_Error PS_SendSoftwareUpdate(SoftwareUpdateFrame* softwareupdateframe, int fd, uint32_t option, uint32_t type)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendSoftwareUpdate(softwareupdateframe, option, type);
	}

	POSITIONSOCKET_API PS_Error PS_SendDeviceStatusSearch(DeviceStatusSearchFrame* devicestatussearchframe, int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->DeviceStatusSearch(devicestatussearchframe);

	}

	POSITIONSOCKET_API PS_Error PS_SendStandardCalib(StandardCalibFrame* standardcalibframe, int fd, uint32_t frqvalue)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendStandardCalib(standardcalibframe, frqvalue);
	}

	POSITIONSOCKET_API PS_Error PS_SendTimeCalib(TimeCalibFrame* timeCalibFrame, int fd, uint32_t ntpAddress)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		auto session = sessiobPool->getSession(fd);
		if (session == nullptr)
			return PS_ERR_INVALID_HANDLE;
		return session->SendTimeCalib(timeCalibFrame, ntpAddress);
	}

	POSITIONSOCKET_API PS_Error PS_CloseConnect(int fd)
	{
		if (sessiobPool == nullptr)
			return PS_ERR_INIT;
		if (fd == -1)
			return PS_ERR_INVALID_HANDLE;
		return sessiobPool->closeHandle(fd);
	}
}
#include "stdafx.h"
#include "ZKChannel.h"
namespace ZBSYB_RADAR_SOCKET
{
	ZKChannel::ZKChannel()
		:DeviceChannel({ 'Z', 'K','\0' })
	{
		m_cmdConverter = make_unique<CmdResponseConverter>();
		m_buffer = make_unique<DeviceStreamBuffer>();
		m_FrameMaker = make_unique<SendFrameMaker>();
		initalResponseFlag();
	}

	ZKChannel::~ZKChannel()
	{
		///m_transport.reset();///20201026 wkd
	}

	void ZKChannel::initalResponseFlag()
	{
		m_responseFlag[SELF_DETECT_ID] = "SelfDetectCmd";
		m_responseFlag[START_STOP_ID] = "StartStopCmd";
		m_responseFlag[LOCK_TRACK_ID] = "LockTrackCmd";
		m_responseFlag[RESET_ID] = "ResetCmd";
		m_responseFlag[PARA_CONFIG_ID] = "ParaConfCmd";
		m_responseFlag[GPS_MODUL_CONFIG_ID] = "GPSModConfCmd";
		m_responseFlag[COMPASS_CALIBRATION_ID] = "CompassCalibCmd";
		m_responseFlag[IP_CONFIG_ID] = "IPConfCmd";
		m_responseFlag[LOW_POWER_CONSUMPTION_ID] = "LowPowerCmd";
		m_responseFlag[WAVE_GATE_SWITCH_ID] = "WaveGateCmd";
		m_responseFlag[AGC_CONTROL_ID] = "AGCCtrlCmd";
		m_responseFlag[DETECT_THRESHOLD_ADJUST_ID] = "DetectThrdCmd";
		m_responseFlag[TIME_CALIBRATION_ID] = "TimeCalibCmd";
		m_responseFlag[SOFTWARE_UPDATE_ID] = "SoftwareUpdateCmd";
		m_responseFlag[DEVICE_STATUS_SEARCH] = "DeviceStatusSearchCmd";
		m_responseFlag[STANDARD_CALIBRATION_ID] = "StandardCalibCmd";
	}

	PS_Error ZKChannel::CreateConnect(const char* peerAddress, const ZKCallbacks& zkcallbacks, void* userPointer)
	{
		PS_Error erro = DeviceChannel::CreateConnect(peerAddress, zkcallbacks.cbCon, userPointer);
		m_zkResultHandler = std::bind(zkcallbacks.cbResult, std::placeholders::_1, userPointer);
		return erro;
	}

	/**
	* @brief 接收数据回调函数，子类自行实现
	*/
	void ZKChannel::SS_OnReceiveComplete(const DataBlock& db)
	{
		auto result = m_buffer->onDeviceData(db);///从数据流提取完整数据帧
		for (auto frame : result)
		{
			auto id = frame->id();
			if (m_cmdConverter->isCmdFrame(id))///回应的是与指令对应的数据帧
			{
				m_cmdConverter->setCurrentFrame(frame);///设置m_cmdConverter中数据帧
				notify();  ///解除阻塞
			}
			else if (m_cmdConverter->isStreamFrame(id))///回应的是与指令对应的数据流
			{
				ZKResult zkResult = frame->makeZKResult();
				if (m_zkResultHandler)
				{
					m_zkResultHandler(&zkResult);
				}
			}
			else
			{
				zsys_debug("error frame id %d", id);
			}
		}
	}

	/**
	* @brief 更新响应标志，唤醒发送指令函数中等待的线程
	* @param[in]  gr 回传工作结果结构体指针
	*/
	void ZKChannel::notify()
	{
		std::lock_guard<std::mutex> lg(m_readyMutex);  ///锁住readymutex
		m_readyCondVar.notify_one();                   ///唤醒等待中的线程
	}

	/**
	* @brief 发送自检指令
	*/
	PS_Error ZKChannel::SendSelfDetectCmd()
	{
		auto block = m_FrameMaker->makeSelfDetectCmdFrame();
		return sendFrame(block);
	}

	PS_Error ZKChannel::SendStartStopCmd(uint32_t option)
	{
		auto block = m_FrameMaker->makeStartStopCmdFrame(option);
		return sendFrame(block);
	}

	PS_Error ZKChannel::SendLockTrackCmd(const LockTrackParam* locktrack)
	{
		auto block = m_FrameMaker->makeLockTrackCmdFrame(*locktrack);
		return sendFrame(block);
	}

	PS_Error ZKChannel::SendResetCmd(ResetFrame* resetframe)
	{
		auto block = m_FrameMaker->makeResetCmdFrame();
		sendFrame(block);
		PS_Error err = wait(RESET_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<ResetFrame>(*resetframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendParaConfCmd(ParamConfFrame* paramconfframe, const ParamConfParam* paramconf)
	{
		auto block = m_FrameMaker->makeParaConfCmdFrame(*paramconf);
		sendFrame(block);
		PS_Error err = wait(PARA_CONFIG_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<ParamConfFrame>(*paramconfframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendGPSModConfCmd(GPSConfFrame* gpsconfframe)
	{
		auto block = m_FrameMaker->makeGPSModConfCmdFrame();
		sendFrame(block);
		PS_Error err = wait(GPS_MODUL_CONFIG_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<GPSConfFrame>(*gpsconfframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendCompassCalibCmd(CompassCalFrame* compasscalframe)
	{
		auto block = m_FrameMaker->makeCompassCalibCmdFrame();
		sendFrame(block);
		PS_Error err = wait(COMPASS_CALIBRATION_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<CompassCalFrame>(*compasscalframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendIPConfCmd(IpConfFrame* ipconfframe, uint32_t address)
	{
		auto block = m_FrameMaker->makeIPConfCmdFrame(address);
		sendFrame(block);
		PS_Error err = wait(IP_CONFIG_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<IpConfFrame>(*ipconfframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendLowPowerCmd(LowPowerConFrame* lowpowerconframe, uint32_t option)
	{
		auto block = m_FrameMaker->makeLowPowerCmdFrame(option);
		sendFrame(block);
		PS_Error err = wait(LOW_POWER_CONSUMPTION_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<LowPowerConFrame>(*lowpowerconframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendWaveGateCmd(WaveGateSwitchFrame* wavegateswitchframe, uint32_t option)
	{
		auto block = m_FrameMaker->makeWaveGateCmdFrame(option);
		sendFrame(block);
		PS_Error err = wait(WAVE_GATE_SWITCH_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<WaveGateSwitchFrame>(*wavegateswitchframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendAGCCtrlCmd(AgcControlFrame* agccontrolframe, uint32_t option, uint32_t value)
	{
		auto block = m_FrameMaker->makeAGCCtrlCmdFrame(option, value);
		sendFrame(block);
		PS_Error err = wait(AGC_CONTROL_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<AgcControlFrame>(*agccontrolframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendDetectThrdCmd(DetectThresholdFrame* detectthresholdframe, uint32_t value)
	{
		auto block = m_FrameMaker->makeDetectThrdCmdFrame(value);
		sendFrame(block);
		PS_Error err = wait(DETECT_THRESHOLD_ADJUST_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<DetectThresholdFrame>(*detectthresholdframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendSoftwareUpdateCmd(SoftwareUpdateFrame* softwareupdateframe, uint32_t option, uint32_t type)
	{
		auto block = m_FrameMaker->makeSoftwareUpdateCmdFrame(option, type);
		sendFrame(block);
		PS_Error err = wait(SOFTWARE_UPDATE_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<SoftwareUpdateFrame>(*softwareupdateframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendDeviceStatusSearchCmd(DeviceStatusSearchFrame* devicestatussearchframe)
	{
		auto block = m_FrameMaker->makeDeviceStatusSearchCmdFrame();
		sendFrame(block);
		PS_Error err = wait(DEVICE_STATUS_SEARCH);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<DeviceStatusSearchFrame>(*devicestatussearchframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendStandardCalibCmd(StandardCalibFrame* standardcalibframe, uint32_t frqvalue)
	{
		auto block = m_FrameMaker->makeStandardCalibCmdFrame(frqvalue);
		sendFrame(block);
		PS_Error err = wait(STANDARD_CALIBRATION_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<StandardCalibFrame>(*standardcalibframe);
		}
		return err;
	}

	PS_Error ZKChannel::SendTimeCalib(TimeCalibFrame* timeCalibFrame,uint32_t ntpAddress)
	{
		auto block = m_FrameMaker->makeTimeCalibCmdFrame(ntpAddress);
		sendFrame(block);
		PS_Error err = wait(TIME_CALIBRATION_ID);
		if (err == PS_ERR_NONE)
		{
			m_cmdConverter->convertToResult<TimeCalibFrame>(*timeCalibFrame);
		}
		return err;

	}

	PS_Error ZKChannel::sendFrame(const DataBlock& block)
	{
		if (m_status != CONNECT)
			return PS_ERR_CONNECT;
		auto err = m_transport->Send(block);
		if (err != PS_ERR_NONE)
			return PS_ERR_SEND;
		return PS_ERR_NONE;
	}

	/**
	* @brief 阻塞线程，等待唤醒
	* @param[in]  timepoint 阻塞时间
	* @return Error
	* @retval ERR_NONE ERR_OVERTIME
	*/
	PS_Error ZKChannel::wait(int flag)
	{
		std::unique_lock<std::mutex> ul(m_readyMutex); ///锁住readymutex
		auto now = std::chrono::system_clock::now();
		if (m_readyCondVar.wait_until(ul, now + TIME_OUT_VALUE,
			[this] {return m_cmdConverter->detectReply(m_FrameMaker->getsequenceNum()); }))///捕获this指针可以访问类的成员变量和成员函数
		{
			//zsys_debug((m_responseFlag[flag] + " Response Succeed").c_str());
			return PS_ERR_NONE;
		}
		else
		{
			//zsys_debug((m_responseFlag[flag] + " Response OverTime").c_str());
			return PS_ERR_OVERTIME;
		}
	}
}
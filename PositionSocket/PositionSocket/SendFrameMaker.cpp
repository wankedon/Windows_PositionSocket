#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "SendFrameMaker.h"
namespace ZBSYB_RADAR_SOCKET
{
	SendFrameMaker::SendFrameMaker()
		: m_sequenceNum(1),
		m_block{ 0,nullptr },
		m_senddata(CMD_FRAME_SIZE, 0xF0F000FF)
	{
		m_senddata[0] = SYNC_WORD_ZK;
		m_senddata[1] = BYTE_LENGTH;
	}

	SendFrameMaker::~SendFrameMaker()
	{
	}

	DataBlock SendFrameMaker::makeSelfDetectCmdFrame()
	{
		return makeSendCmdFrame(SELF_DETECT_ID);
	}

	DataBlock SendFrameMaker::makeStartStopCmdFrame(uint32_t option)
	{
		m_senddata[4] = option;
		return makeSendCmdFrame(START_STOP_ID);
	}

	DataBlock SendFrameMaker::makeLockTrackCmdFrame(const LockTrackParam& locktrack)
	{
		uint32_t* p = (uint32_t*)&locktrack;
		for (int i = 0; i < sizeof(locktrack) / sizeof(uint32_t); i++)
		{
			m_senddata[4 + i] = *p++;
		}
		return makeSendCmdFrame(LOCK_TRACK_ID);
	}

	DataBlock SendFrameMaker::makeResetCmdFrame()
	{
		return makeSendCmdFrame(RESET_ID);
	}

	DataBlock SendFrameMaker::makeParaConfCmdFrame(const ParamConfParam& paramconf)
	{
		uint32_t* p = (uint32_t*)&paramconf;
		for (int i = 0; i < sizeof(paramconf) / sizeof(uint32_t); i++)
		{
			m_senddata[4 + i] = *p++;
		}
		return makeSendCmdFrame(PARA_CONFIG_ID);
	}

	DataBlock SendFrameMaker::makeGPSModConfCmdFrame()
	{
		return makeSendCmdFrame(GPS_MODUL_CONFIG_ID);
	}

	DataBlock SendFrameMaker::makeCompassCalibCmdFrame()
	{
		return makeSendCmdFrame(COMPASS_CALIBRATION_ID);
	}

	DataBlock SendFrameMaker::makeIPConfCmdFrame(uint32_t address)
	{
		m_senddata[4] = address;
		return makeSendCmdFrame(IP_CONFIG_ID);
	}

	DataBlock SendFrameMaker::makeLowPowerCmdFrame(uint32_t option)
	{
		m_senddata[4] = option;
		return makeSendCmdFrame(LOW_POWER_CONSUMPTION_ID);
	}

	DataBlock SendFrameMaker::makeWaveGateCmdFrame(uint32_t option)
	{
		m_senddata[4] = option;
		return makeSendCmdFrame(WAVE_GATE_SWITCH_ID);
	}

	DataBlock SendFrameMaker::makeAGCCtrlCmdFrame(uint32_t option, uint32_t value)
	{
		m_senddata[4] = option;
		m_senddata[5] = value;
		return makeSendCmdFrame(AGC_CONTROL_ID);
	}

	DataBlock SendFrameMaker::makeDetectThrdCmdFrame(uint32_t value)
	{
		m_senddata[4] = value;
		return makeSendCmdFrame(DETECT_THRESHOLD_ADJUST_ID);
	}

	DataBlock SendFrameMaker::makeSoftwareUpdateCmdFrame(uint32_t option, uint32_t type)
	{
		m_senddata[4] = option;
		m_senddata[5] = type;
		return makeSendCmdFrame(SOFTWARE_UPDATE_ID);
	}

	DataBlock SendFrameMaker::makeDeviceStatusSearchCmdFrame()
	{
		return makeSendCmdFrame(DEVICE_STATUS_SEARCH);
	}

	DataBlock SendFrameMaker::makeStandardCalibCmdFrame(uint32_t frqvalue)
	{
		m_senddata[4] = frqvalue;
		return makeSendCmdFrame(STANDARD_CALIBRATION_ID);
	}

	DataBlock SendFrameMaker::makeTimeCalibCmdFrame(uint32_t ntpAddress)
	{
		m_senddata[4] = ntpAddress;
		return makeSendCmdFrame(TIME_CALIBRATION_ID);
	}

	DataBlock SendFrameMaker::makeSendCmdFrame(uint32_t cmdId)
	{
		m_senddata[2] = cmdId;
		m_senddata[3] = m_sequenceNum;
		m_sequenceNum++;
		m_senddata[CMD_FRAME_SIZE - 1] = calculateChecksum();
		m_block.len = int(m_senddata.size() * sizeof(uint32_t));
		m_block.addr = m_senddata.data();
		return m_block;
	}

	uint32_t SendFrameMaker::getsequenceNum()
	{
		return m_sequenceNum - 1;
	}

	uint32_t SendFrameMaker::calculateChecksum()
	{
		int bufferuintlength = BYTE_LENGTH / sizeof(uint32_t) - 1;
		vector<byte> bufferbyte(BYTE_LENGTH - sizeof(uint32_t));
		vector<uint32_t> bufferuint(bufferuintlength);
		bufferuint.at(0) = m_senddata[2];
		for (int i = 0; i < bufferuintlength - 1; i++)
		{
			bufferuint.at(i + 1) = m_senddata[i + 4];
		}
		memcpy(bufferbyte.data(), bufferuint.data(), bufferuint.size() * sizeof(uint32_t));
		uint32_t checkSumtemp = 0;
		for (int i = 0; i < bufferbyte.size(); i++)
		{
			checkSumtemp = checkSumtemp + bufferbyte[i];
		}
		return checkSumtemp;
	}
}
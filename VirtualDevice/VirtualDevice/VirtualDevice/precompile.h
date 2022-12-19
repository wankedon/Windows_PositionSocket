#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <czmq.h>
#include <map>

#include <set>
#include "SuperSocketAPI.h"



using namespace SuperSocketLib;
/**
* @brief ����֡����ID��
*/
enum WorkCommand
{
	SYNC_WORD_ZK = 0X7E7E4141,                ///< �ش����ƽ������֡ͬ����
	SYNC_WORD_YC = 0XA5A54141,                ///< �ش�ң������֡ͬ����
	TOTAL_BYTE_LENGTH_128 = 128,              ///< �ش����ƽ������֡�ֽڳ���ֵ
	TOTAL_BYTE_LENGTH_256 = 256,              ///< �ش����ƽ������֡�ֽڳ���ֵ
	NO_BYTE_LENGTH = 12,                      ///< �ش����ƽ������֡�ֽڳ���ֵ
	TOTAL_BYTE_LENGTH_YC = 392,               ///< �ش�ң������֡�ֽڳ���ֵ
	SELF_DETECT_ID = 0X10000001,	          ///< �Լ�ָ��
	START_STOP_ID = 0X10000002,	              ///< ����/ָֹͣ��
	LOCK_TRACK_ID = 0X10000003,	              ///< ��������
	RESET_ID = 0X10000004,	                  ///< ��λָ��
	PARA_CONFIG_ID = 0X10000005,			  ///< ��������ָ��
	GPS_MODUL_CONFIG_ID = 0X10000006,	      ///< GPSģ������
	COMPASS_CALIBRATION_ID = 0X10000007,	  ///< ��������У׼
	IP_CONFIG_ID = 0X10000008,			      ///< ���ڵ�IP����
	LOW_POWER_CONSUMPTION_ID = 0X10000009,    ///< �͹���ָ��
	WAVE_GATE_SWITCH_ID = 0X20000001,		  ///< ���ſ���
	AGC_CONTROL_ID = 0X20000002,			  ///< AGC����
	DETECT_THRESHOLD_ADJUST_ID = 0X20000003,  ///< ������޵���
	TIME_CALIBRATION_ID = 0X20000004,		  ///< ʱ��У׼
	SOFTWARE_UPDATE_ID = 0X20000005,		  ///< �������
	STANDARD_CALIBRATION_ID = 0X30000001,	  ///< ��Уָ��
	SET_TO_IDLE_ID = 0x20000006,
};

/**
* @brief ��������֡�ṹ��
*/
struct CommonSendHeader
{
	uint32_t syncword;
	uint32_t byteLength;
};

struct recieveDataFrame
{
	CommonSendHeader header;
	uint32_t commandID;
	uint32_t counter;
	uint32_t dataBlock[27];
	uint32_t checkSum;
};

using namespace std;
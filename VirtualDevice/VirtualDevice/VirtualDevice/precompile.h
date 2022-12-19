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
* @brief 数据帧命令ID号
*/
enum WorkCommand
{
	SYNC_WORD_ZK = 0X7E7E4141,                ///< 回传控制结果数据帧同步字
	SYNC_WORD_YC = 0XA5A54141,                ///< 回传遥测数据帧同步字
	TOTAL_BYTE_LENGTH_128 = 128,              ///< 回传控制结果数据帧字节长度值
	TOTAL_BYTE_LENGTH_256 = 256,              ///< 回传控制结果数据帧字节长度值
	NO_BYTE_LENGTH = 12,                      ///< 回传控制结果数据帧字节长度值
	TOTAL_BYTE_LENGTH_YC = 392,               ///< 回传遥测数据帧字节长度值
	SELF_DETECT_ID = 0X10000001,	          ///< 自检指令
	START_STOP_ID = 0X10000002,	              ///< 启动/停止指令
	LOCK_TRACK_ID = 0X10000003,	              ///< 锁定跟踪
	RESET_ID = 0X10000004,	                  ///< 复位指令
	PARA_CONFIG_ID = 0X10000005,			  ///< 参数配置指令
	GPS_MODUL_CONFIG_ID = 0X10000006,	      ///< GPS模块配置
	COMPASS_CALIBRATION_ID = 0X10000007,	  ///< 电子罗盘校准
	IP_CONFIG_ID = 0X10000008,			      ///< 监测节点IP配置
	LOW_POWER_CONSUMPTION_ID = 0X10000009,    ///< 低功耗指令
	WAVE_GATE_SWITCH_ID = 0X20000001,		  ///< 波门开关
	AGC_CONTROL_ID = 0X20000002,			  ///< AGC控制
	DETECT_THRESHOLD_ADJUST_ID = 0X20000003,  ///< 检测门限调整
	TIME_CALIBRATION_ID = 0X20000004,		  ///< 时间校准
	SOFTWARE_UPDATE_ID = 0X20000005,		  ///< 软件更新
	STANDARD_CALIBRATION_ID = 0X30000001,	  ///< 标校指令
	SET_TO_IDLE_ID = 0x20000006,
};

/**
* @brief 接收数据帧结构体
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
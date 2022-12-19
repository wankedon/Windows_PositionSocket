/**
* @file stdafx.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief 预编译头文件

* @author wangkedong

* @date 2020:06:04

* @note 

* @version <1.0.0>

*/
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include "supersocket.h"
#include "SuperSocketAPI.h"
#include <functional>
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include "assert.h"
#include <czmq.h>
#include <mutex>
#include <condition_variable>
using namespace std;
namespace ZBSYB_RADAR_SOCKET
{
	/**
	* @brief 命令ID号
	*/
	enum CommandID
	{
		SYNC_WORD_ZK = 0X7E7E4141,
		SYNC_WORD_YC = 0XA5A54141,
		BYTE_LENGTH = 116,
		SELF_DETECT_ID = 0X10000001,	         ///< 自检指令
		START_STOP_ID = 0X10000002,	             ///< 启动/停止指令
		LOCK_TRACK_ID = 0X10000003,	             ///< 锁定跟踪
		RESET_ID = 0X10000004,	                 ///< 复位指令
		PARA_CONFIG_ID = 0X10000005,			 ///< 参数配置指令
		GPS_MODUL_CONFIG_ID = 0X10000006,	     ///< GPS模块配置
		COMPASS_CALIBRATION_ID = 0X10000007,	 ///< 电子罗盘校准
		IP_CONFIG_ID = 0X10000008,			     ///< 监测节点IP配置
		LOW_POWER_CONSUMPTION_ID = 0X10000009,   ///< 低功耗指令
		WAVE_GATE_SWITCH_ID = 0X20000001,		 ///< 波门开关
		AGC_CONTROL_ID = 0X20000002,			 ///< AGC控制
		DETECT_THRESHOLD_ADJUST_ID = 0X20000003, ///< 检测门限调整
		TIME_CALIBRATION_ID = 0X20000004,	     ///< 对时控制
		SOFTWARE_UPDATE_ID = 0X20000005,		 ///< 软件更新
		DEVICE_STATUS_SEARCH = 0X20000006,       ///< 设备状态查询
		STANDARD_CALIBRATION_ID = 0X30000001,	 ///< 标校指令
	};

	// TODO: reference additional headers your program requires here
}
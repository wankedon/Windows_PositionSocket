#include "stdafx.h"
#include "CmdResponseConverter.h"
namespace ZBSYB_RADAR_SOCKET
{
	const set<uint32_t> CmdResponseConverter::CMD_NAME =
	{
		RESET_ID,                    ///< 复位指令
		PARA_CONFIG_ID,		         ///< 参数配置指令
		GPS_MODUL_CONFIG_ID,         ///< GPS模块配置
		COMPASS_CALIBRATION_ID,	     ///< 电子罗盘校准
		IP_CONFIG_ID,			     ///< 监测节点IP配置
		LOW_POWER_CONSUMPTION_ID,    ///< 低功耗指令
		WAVE_GATE_SWITCH_ID,	     ///< 波门开关
		AGC_CONTROL_ID,		         ///< AGC控制
		DETECT_THRESHOLD_ADJUST_ID,  ///< 检测门限调整
		TIME_CALIBRATION_ID,		 ///< 对时控制
		SOFTWARE_UPDATE_ID,		     ///< 软件更新
		DEVICE_STATUS_SEARCH,        ///< 设备状态查询
		STANDARD_CALIBRATION_ID,     ///< 标校指令
	};

	const set<uint32_t> CmdResponseConverter::STREAM_NAME =
	{
		SELF_DETECT_ID,              ///< 自检指令
		START_STOP_ID,               ///< 启动/停止指令
		LOCK_TRACK_ID,               ///< 锁定跟踪
	};
}
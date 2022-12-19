#include "stdafx.h"
#include "CmdResponseConverter.h"
namespace ZBSYB_RADAR_SOCKET
{
	const set<uint32_t> CmdResponseConverter::CMD_NAME =
	{
		RESET_ID,                    ///< ��λָ��
		PARA_CONFIG_ID,		         ///< ��������ָ��
		GPS_MODUL_CONFIG_ID,         ///< GPSģ������
		COMPASS_CALIBRATION_ID,	     ///< ��������У׼
		IP_CONFIG_ID,			     ///< ���ڵ�IP����
		LOW_POWER_CONSUMPTION_ID,    ///< �͹���ָ��
		WAVE_GATE_SWITCH_ID,	     ///< ���ſ���
		AGC_CONTROL_ID,		         ///< AGC����
		DETECT_THRESHOLD_ADJUST_ID,  ///< ������޵���
		TIME_CALIBRATION_ID,		 ///< ��ʱ����
		SOFTWARE_UPDATE_ID,		     ///< �������
		DEVICE_STATUS_SEARCH,        ///< �豸״̬��ѯ
		STANDARD_CALIBRATION_ID,     ///< ��Уָ��
	};

	const set<uint32_t> CmdResponseConverter::STREAM_NAME =
	{
		SELF_DETECT_ID,              ///< �Լ�ָ��
		START_STOP_ID,               ///< ����/ָֹͣ��
		LOCK_TRACK_ID,               ///< ��������
	};
}
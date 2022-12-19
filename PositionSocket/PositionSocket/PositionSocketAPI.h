/**
* @file PositionSocketAPI.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief PositionSocket库基础定义，包含C API

* @author wangkedong

* @date 2020:07:09

* @note 

* @version <1.0.0>

*/

/**
* @mainpage PositionSocket动态库
* @section PositionSocketAPI简介：
* @subsection PositionSocketAPI为便携式电磁辐射源侦查定位系统显控终端与监测节点进行数据通信的接口
* @subsection 主要用于显控终端向监测节点下发指令，并接受收监测节点回传指令结果及测量数据。
* @section CAPI PositionSocketAPI 使用方法：
* @subsection step1 Step 1: 引用PositionSocketAPI.h
* @subsection step2 Step 2: 调用PS_InitSSLib()初始化库
* @subsection step3 Step 3: 定义回调函数、设置ip地址、调用PS_CreateConnect()建立连接，获得句柄fd描述符
* @subsection step4 Step 4: 利用该句柄fd发送数据，通过回调函数接收数据
* @subsection step5 Step 5: 不再传输数据时，调用PS_CloseConnect()关闭连接
* @subsection step6 Step 6: 不再使用库时，调用PS_DeinitSSLib()关闭库，释放资源
*/
#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#ifdef POSITIONSOCKET_EXPORTS
#define POSITIONSOCKET_API __declspec(dllexport)
#else
#define POSITIONSOCKET_API __declspec(dllimport)
#endif

namespace ZBSYB_RADAR_SOCKET
{
		/**
		* @brief API返回值，错误码标识
		*/
		enum PS_Error
		{
			PS_ERR_NONE = 0,	            ///<无错误
			PS_ERR_INIT = -1,               ///<初始化库错误
			PS_ERR_INVALID_HANDLE = -2,     ///<无效句柄
			PS_ERR_CONNECT = -3,            ///<连接错误
			PS_ERR_SEND = -4,               ///<发送错误
			PS_ERR_OVERTIME = -5,           ///<响应超时
			PS_ERR_NO_HANDLE_RESOURCE = -6, ///<无句柄资源
			PS_ERR_CREATE_SOCKET = -7,      ///<创建SOCKET错误
		};

#pragma pack(1)
		/**
		* @brief 综控结果回传数据帧帧头
		*/
		struct FrameHeader
		{
			uint32_t syncWord;        ///<同步字  
			uint32_t byteLength;      ///<字节长度值
			uint32_t commandID;       ///<命令ID号
			uint32_t response;        ///<指令响应标志
			uint32_t counter;         ///<指令响应次数累加值
			uint32_t systemTime;      ///<系统时标
		};

		/**
		* @brief 自检结果回传数据帧
		*/
		struct SelfDetectFrame
		{
			FrameHeader header;
			float GPSheight;         ///<GPS海拔
			float GPSlat;            ///<GPS纬度
			float GPSlong;           ///<GPS经度
			uint8_t S_or_N;           ///<GPS南北纬
			uint8_t E_or_W;           ///<GPS东西经
			uint8_t statelite;        ///<GPS卫星数
			uint8_t hour;             ///<GPS时
			uint8_t minute;           ///<GPS分
			uint8_t sec;              ///<GPS秒
			uint8_t day;              ///<GPS日
			uint8_t month;            ///<GPS月
			uint16_t year;            ///<GPS年
			uint16_t obligate1;       ///<预留
			uint32_t obligate2;       ///<预留
			uint32_t RFSOC_PS_ver;    ///<RFSOC_PS程序版本
			uint32_t RFSOC_PL_ver;    ///<RFSOC_PL程序版本
			uint32_t RFSOC_det;       ///<RFSOC接口自检
			uint32_t DSP_ver;         ///<DSP程序版本
			uint32_t DSP_det;         ///<DSP硬件接口自检
			uint32_t microWave_det;   ///<微波接收机自检
			uint32_t temperature;     ///<温度 无效
			uint32_t reserved[11];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 启动/停止结果回传数据帧
		*/
		struct StartStopFrame
		{
			FrameHeader header;
			float GPSheight;          ///<GPS海拔
			float GPSlat;             ///<GPS纬度
			float GPSlong;            ///<GPS经度
			uint8_t SorN;             ///<GPS南北纬
			uint8_t EorW;             ///<GPS东西经
			uint8_t statelite;        ///<GPS卫星数
			uint8_t hour;             ///<GPS时
			uint8_t minute;           ///<GPS分
			uint8_t sec;              ///<GPS秒
			uint8_t day;              ///<GPS日
			uint8_t month;            ///<GPS月
			uint16_t year;            ///<GPS年
			uint16_t msec;            ///<毫秒
			uint32_t obligate2;       ///<预留
			uint32_t frq;             ///<目标频率
			float azi;                ///<目标方位角
			float ele;                ///<目标俯仰角
			uint32_t LO_RF;           ///<当前搜索频段本振
			uint32_t scan_index;      ///<扫描序列号
			int32_t amp;              ///<目标信号功率 int32_t
			uint32_t type;            ///<目标调制格式
			uint32_t fre_num;         ///<目标载频种类数
			uint32_t PW_num;          ///<目标脉宽种类数
			uint32_t PRI_num;         ///<目标重复周期种类数
			uint16_t fre[10];         ///<载频参数值
			uint16_t pw[10];          ///<脉宽参数值
			uint16_t pri[10];         ///<重复周期参数值
			uint16_t temperature;     ///<硬件温度
			uint16_t Vcc_PL;          ///<电压
			uint16_t Vcc_LOC;         ///<电压
			uint16_t obligate3;       ///<预留
			float head;               ///<GI方位
			float pitch;              ///<GI俯仰
			float roll;               ///<横滚
			float azi_j0;             ///<解耦后的方位角
			float ele_j0;             ///<解耦后的俯仰角
			float amp_max;            ///<幅度最大值
			float amp_min;            ///<幅度最小值
			float amp_mean;           ///<幅度均值
			uint16_t grid_pulse_num;    ///<当前类中包含的脉冲个数
			uint16_t cluster_pulse_num; ///<前参与聚类分选的总脉冲个数
			uint16_t cw_yn;             ///<连续波标志0脉冲，1连续波
			uint16_t frq_max;           ///<当前类中频率最大值
			uint16_t frq_min;           ///<当前类中频率最小值
			uint16_t usResv1;           ///<预留
			uint32_t reserved[12];      ///<预留
			uint32_t checkSum;          ///<校验和
		};

		/**
		* @brief 锁定跟踪结果回传数据帧
		*/
		struct LockTrackFrame
		{
			FrameHeader header;
			float GPSheight;          ///<GPS海拔
			float GPSlat;             ///<GPS纬度
			float GPSlong;            ///<GPS经度
			uint8_t SorN;             ///<GPS南北纬
			uint8_t EorW;             ///<GPS东西经
			uint8_t statelite;        ///<GPS卫星数
			uint8_t hour;             ///<GPS时
			uint8_t minute;           ///<GPS分
			uint8_t sec;              ///<GPS秒
			uint8_t day;              ///<GPS日
			uint8_t month;            ///<GPS月
			uint16_t year;            ///<GPS年
			uint16_t obligate1;       ///<预留
			uint32_t obligate2;       ///<预留
			uint32_t frq;             ///<目标频率
			float azi;                ///<目标方位角
			float ele;                ///<目标俯仰角
			uint32_t LO_RF;           ///<当前搜索频段本振
			uint32_t scan_index;      ///<扫描序列号
			int32_t amp;              ///<目标信号功率
			uint32_t type;            ///<目标调制格式
			uint32_t fre_num;         ///<目标载频种类数
			uint32_t PW_num;          ///<目标脉宽种类数
			uint32_t PRI_num;         ///<目标重复周期种类数
			uint16_t fre[10];         ///<载频参数值
			uint16_t pw[10];          ///<脉宽参数值
			uint16_t pri[10];         ///<重复周期参数值
			uint16_t temperature;     ///<硬件温度
			uint16_t Vcc_PL;          ///<电压
			uint16_t Vcc_LOC;         ///<电压
			uint16_t obligate3;       ///<预留
			float head;               ///<GI方位
			float pitch;              ///<GI俯仰
			float roll;               ///<横滚
			float azi_j0;             ///<解耦后的方位角
			float ele_j0;             ///<解耦后的俯仰角
			float amp_max;            ///<幅度最大值
			float amp_min;            ///<幅度最小值
			float amp_mean;           ///<幅度均值
			uint32_t reserved[15];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 复位结果回传数据帧
		*/
		struct ResetFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 参数配置结果回传数据帧
		*/
		struct ParamConfFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief GPS模块配置结果回传数据帧
		*/
		struct GPSConfFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 电子罗盘校准结果回传数据帧
		*/
		struct CompassCalFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 监测节点IP配置结果回传指令数据帧
		*/
		struct IpConfFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 低功耗结果回传数据帧
		*/
		struct LowPowerConFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 波门控制结果回传数据帧
		*/
		struct WaveGateSwitchFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief AGC控制结果回传数据帧
		*/
		struct AgcControlFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 检测门限控制结果回传数据帧
		*/
		struct DetectThresholdFrame
		{
			FrameHeader header;
			uint32_t reserved[25];    ///<预留
			uint32_t checkSum;        ///<校验和
		};

		/**
		* @brief 软件更新控制结果回传数据帧
		*/
		struct SoftwareUpdateFrame
		{
			FrameHeader header;
			uint32_t reserved[25];   ///<预留
			uint32_t checkSum;       ///<校验和
		};

		/**
		* @brief 设备状态查询结果回传数据帧
		*/
		struct DeviceStatusSearchFrame
		{
			FrameHeader header;
			uint32_t reserved[25];   ///<预留
			uint32_t checkSum;       ///<校验和
		};

		/**
		* @brief 标校指令控制结果回传数据帧
		*/
		struct StandardCalibFrame
		{
			FrameHeader header;
			uint32_t Fre;
			uint32_t reserved[24];  ///<预留
			uint32_t checkSum;		///<校验和
		};

		/**
        * @brief 对时指令控制结果回传数据帧
        */
		struct TimeDescribe
		{
			uint32_t sec;   ///<时间秒
			uint32_t msec;  ///<时间毫秒
		};

		struct TimeDiff
		{
			int32_t sec;   ///<时间秒
			int32_t msec;  ///<时间毫秒
		};

		struct TimeCalibFrame
		{
			FrameHeader header;  
			TimeDescribe T1_time;   ///<T1时间
			TimeDescribe T2_time;   ///<T2时间
			TimeDescribe T3_time;   ///<T3时间
			TimeDescribe T4_time;   ///<T4时间
			TimeDiff Diff_time;     ///<时间差 
			float GPSheight;         ///<GPS海拔
			float GPSlat;            ///<GPS纬度
			float GPSlong;           ///<GPS经度
			uint8_t S_or_N;           ///<GPS南北纬
			uint8_t E_or_W;           ///<GPS东西经
			uint8_t statelite;        ///<GPS卫星数
			uint8_t hour;             ///<GPS时
			uint8_t minute;           ///<GPS分
			uint8_t sec;              ///<GPS秒
			uint8_t day;              ///<GPS日
			uint8_t month;            ///<GPS月
			uint16_t year;            ///<GPS年
			uint16_t msec;            ///<GPS毫秒
			uint32_t reserved[9];     ///<预留
			uint32_t checkSum;	      ///<校验和
		};

		/**
		* @brief 锁定跟踪指令参数
		*/
		struct LockTrackParam
		{
			uint32_t listCount;          ///<搜索起始频率
			uint32_t indexNum;           ///<搜索结束频率
			uint32_t startFrequency;     ///<搜索起始频率
			uint32_t endFrequency;       ///<搜索结束频率
			uint32_t searchTime;         ///<搜索压制时间
			uint32_t searchBand;         ///<搜索带宽
			uint32_t pulseWidthStart;    ///<脉宽上限值
			uint32_t pulseWidthEnd;      ///<脉宽下限值
			uint32_t periodRepeatStart;  ///<重复周期上限值
			uint32_t periodRepeatEnd;    ///<重复周期下限值
			uint32_t targetSignalType;   ///<目标信号类型
		};

		/**
		* @brief 参数配置指令参数
		*/
		struct ParamConfParam
		{
			uint32_t listCount;          ///<搜索起始频率
			uint32_t indexNum;           ///<搜索结束频率
			uint32_t startFrequency;     ///<搜索起始频率
			uint32_t endFrequency;       ///<搜索结束频率
			uint32_t searchTime;         ///<搜索压制时间
			uint32_t searchBand;         ///<搜索带宽
			uint32_t pulseWidthStart;    ///<脉宽上限值
			uint32_t pulseWidthEnd;      ///<脉宽下限值
			uint32_t periodRepeatStart;  ///<重复周期上限值
			uint32_t periodRepeatEnd;    ///<重复周期下限值
			uint32_t targetSignalType;   ///<目标信号类型
			uint32_t noiseThreshold;     ///<噪声门限
		};
#pragma pack()

		/**
		* @brief 通道状态
		*/
		enum ChannelState
		{
			CONNECTED,     ///<连接
			DISCONNECTED,  ///<断开链接
		};

		/**
		* @brief 回传终控结果数据帧
		*/
		struct ZKResult
		{
			SelfDetectFrame* selfdetectframe; ///<自检结果
			StartStopFrame* startstopframe;   ///<启动停止结果
			LockTrackFrame* locktrackframe;   ///<锁定跟踪结果
		};

		/**
		* @brief 回传遥测数据帧
		*/
		struct YCResult
		{
			size_t length;   ///<遥测数据长度
			uint32_t* data;  ///<遥测数据起始地址
		};

		/**
		* @brief 自定义类型
		*/
		typedef void CHANNEL_STATE_CALLBACK(ChannelState state, void* userPointer);
		typedef void ZK_RESULT_CALLBACK(const ZKResult* zkResult, void* userPointer);
		typedef void YC_RESULT_CALLBACK(const YCResult* ycResult, void* userPointer);

		/**
		* @brief 综控回调函数集合
		*/
		struct ZKCallbacks
		{
			CHANNEL_STATE_CALLBACK* cbCon;
			ZK_RESULT_CALLBACK* cbResult;
		};

		/**
		* @brief 遥测回调函数集合
		*/
		struct YCCallbacks
		{
			CHANNEL_STATE_CALLBACK* cbCon;
			YC_RESULT_CALLBACK* cbResult;
		};

		/**
		* @brief 回调函数集合
		*/
		struct SessionHandler
		{
			void* userPointer;
			ZKCallbacks cbZK;
			YCCallbacks cbYC;
		};

		/**
		* @brief 初始化库 使用库时，第一个调用的函数，仅调用一次
		*/
		POSITIONSOCKET_API PS_Error PS_InitSSLib();

		/**
		* @brief 关闭库，最后一个调用的函数，仅调用一次
		* @details 调用此函数之后，除了PS_InitSSLib(),不能再调用其他C接口函数
		*/
		POSITIONSOCKET_API PS_Error PS_DeinitSSLib();

		/**
		* @brief 建立连接
		* @param[out] fd 句柄
		* @param[in] peerAddress 服务器地址IP和端口字符串，例如"192.168.1.33:8080"
		* @param[in] handler 回调函数集合，回调函数由客户开发，对应响应函数
		* @return Error
		* @retval ERR_NONE ERR_CONNECT
		*/
		POSITIONSOCKET_API PS_Error PS_CreateConnect(int* fd, const char* peerAddress, const SessionHandler* handler);

		/**
		* @brief 发送自检指令
		* @param[out] selfdetectframe 自检回传结果
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendSelfDetect(int fd);

		/**
		* @brief 发送启动/停止指令
		* @param[in] fd 句柄
		* @param[in] option 1：启动测量；0:停止测量
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendStartStop(int fd, uint32_t option);

		/**
		* @brief 发送锁定跟踪指令
		* @param[in] fd 句柄
		* @param[in] locktrack 锁定跟踪指令参数
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendLockTrack(int fd, const LockTrackParam* locktrack);

		/**
		* @brief 发送复位指令
		* @param[out] resetframe 复位回传结果
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendReset(ResetFrame* resetframe, int fd);

		/**
		* @brief 发送参数配置指令
		* @param[out] paramconfframe 参数配置回传结果
		* @param[in] fd 句柄
		* @param[in] paramconf 参数配置指令参数
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendParaConf(ParamConfFrame* paramconfframe, int fd, const ParamConfParam* paramconf);

		/**
		* @brief 发送GPS模块配置指令
		* @param[out] gpsconfframe GPS模块配置回传结果
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendGPSModConf(GPSConfFrame* gpsconfframe, int fd);

		/**
		* @brief 发送电子罗盘校准指令
		* @param[out] compasscalframe 电子罗盘校准回传结果
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendCompassCalib(CompassCalFrame* compasscalframe, int fd);

		/**
		* @brief 发送监测节点IP配置指令
		* @param[out] ipconfframe  监测节点IP配置回传结果
		* @param[in] fd 句柄
		* @param[in] address IP地址
		* @return Error
		* @retval ERR_NONE
		*/
		POSITIONSOCKET_API PS_Error PS_SendIPConf(IpConfFrame* ipconfframe, int fd, uint32_t address);

		/**
		* @brief 发送低功耗指令
		* @param[out] lowpowerconframe  低功耗回传结果
		* @param[in] fd 句柄
		* @param[in] option 1：进入低功耗；0:退出低功耗
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendLowPower(LowPowerConFrame* lowpowerconframe, int fd, uint32_t option);

		/**
		* @brief 发送波门控制指令
		* @param[out] wavegateswitchframe  波门控制回传结果
		* @param[in] fd 句柄
		* @param[in] option 1：开；0:关
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendWaveGate(WaveGateSwitchFrame* wavegateswitchframe, int fd, uint32_t option);

		/**
		* @brief 发送AGC控制指令
		* @param[out] agccontrolframe  AGC控制回传结果
		* @param[in] fd 句柄
		* @param[in] option 1：开环；0:闭环
		* @param[in] agcvalue AGC码值
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendAGCCtrl(AgcControlFrame* agccontrolframe, int fd, uint32_t option, uint32_t agcvalue);

		/**
		* @brief 发送检测门限控制指令
		* @param[out] detectthresholdframe  检测门限控制回传结果
		* @param[in] fd 句柄
		* @param[in] value 检测门限值
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendDetectThrd(DetectThresholdFrame* detectthresholdframe, int fd, uint32_t value);

		/**
		* @brief 发送软件更新指令
		* @param[out] softwareupdateframe  软件更新控制回传结果
		* @param[in] fd 句柄
		* @param[in] option 软件更新开始结束指令；0xAA：开始；0xBB:结束
		* @param[in] type 软件类型 0xAA:FPGA软件；0xDD:DSP软件 0xCA:校准数据
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendSoftwareUpdate(SoftwareUpdateFrame* softwareupdateframe, int fd, uint32_t option, uint32_t type);

		/**
		* @brief 发送设备状态查询指令
		* @param[out] devicestatussearchframe 设备状态查询回传结果
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendDeviceStatusSearch(DeviceStatusSearchFrame* devicestatussearchframe, int fd);

		/**
		* @brief 发送标校控制指令
		* @param[out] standardcalibframe 标校指令控制回传结果
		* @param[in] fd 句柄
		* @param[in] frqvalue 频率值
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendStandardCalib(StandardCalibFrame* standardcalibframe, int fd, uint32_t frqvalue);

		/**
		* @brief 发送对时指令
		* @param[out] timeCalibFrame 对时指令控制回传结果
		* @param[in] fd 句柄
		* @param[in] ntpAddress 授时服务器ip
		* @return Error
		* @retval ERR_NONE ERR_SEND ERR_OVERTIME
		*/
		POSITIONSOCKET_API PS_Error PS_SendTimeCalib(TimeCalibFrame* timeCalibFrame, int fd, uint32_t ntpAddress);

		/**
		* @brief 关闭连接
		* @param[in] fd 句柄
		* @return Error
		* @retval ERR_NONE
		*/
		POSITIONSOCKET_API PS_Error PS_CloseConnect(int fd);
}

#ifdef __cplusplus
}
#endif
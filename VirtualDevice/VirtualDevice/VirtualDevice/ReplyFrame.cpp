#include "precompile.h"
#include "ReplyFrame.h"
#include "SuperSocketAPI.h"
#include "VirtualDevice.h"
#include <random>
#include <time.h>
#include "UTCTimeConvertTools.h"
using namespace UTC_CONVERT;
#define  SIGNAL_NUMBER 100

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> segDis(0, SIGNAL_NUMBER-1);
const std::map<uint32_t, string> ReplyFrame::CMD_NAME = {
	{SELF_DETECT_ID,			"�Լ�"},
	{START_STOP_ID,				"����ֹͣ"},
	{LOCK_TRACK_ID,				"��������"},
	{RESET_ID,					"��λ"},
	{PARA_CONFIG_ID,			"��������"},
	{GPS_MODUL_CONFIG_ID,		"GPSģ������"},
	{COMPASS_CALIBRATION_ID,	"��������У׼"},
	{IP_CONFIG_ID,				"���ڵ�IP����"},
	{LOW_POWER_CONSUMPTION_ID,	"�͹���ָ��"},
	{WAVE_GATE_SWITCH_ID,		"���ſ���"},
	{AGC_CONTROL_ID,			"AGC����"},
	{DETECT_THRESHOLD_ADJUST_ID,"������޵���"},
	{TIME_CALIBRATION_ID,		"ʱ��У׼"},
	{SOFTWARE_UPDATE_ID,		"�������"},
	{STANDARD_CALIBRATION_ID,	"��У"},
	{SET_TO_IDLE_ID,			"����"},
};


ReplyFrame::ReplyFrame(const vector<byte>& cmd)
{
	if (cmd.size() < REPLY_FRAME_SIZE * 4)
		return;
	data = make_unique<vector<uint32_t>>(REPLY_FRAME_SIZE, 0x5a5ac3c3);
	memcpy(data->data(), cmd.data(), 16);//����ͬ���֣����ݳ��ȣ�����id, ����˳���
	(*data)[4] = (*data)[3];	//������˳���Ų����5���ֶ�
	(*data)[3] = 1;	//��4���ֶα�Ϊ��Ӧȷ��
	(*data)[5] = (uint32_t)time(nullptr);

	float gps_height = 1000.0;
	float gps_lat = 35.7;
	float gps_long = 120.7;
	memcpy(&data->at(6), &gps_height, sizeof(float)); /// [7]
	memcpy(&data->at(7), &gps_lat, sizeof(float)); /// [8]
	memcpy(&data->at(8), &gps_long, sizeof(float)); /// [9]
	if ((*data)[2] == TIME_CALIBRATION_ID)
	{
		(*data)[6] = 20;
		(*data)[14] = 0;
		(*data)[15] = 15;
		//ʱ��
		auto utc = getCurrentUtc();
		vector<uint8_t> hour(4);
		hour[0] = 35;
		hour[1] = 120;
		hour[2] = 520;
		hour[3] = utc.hour;
		memcpy(&data->at(19), hour.data(), sizeof(uint8_t) * 4);
		vector<uint8_t> minute(4);
		minute[0] = utc.min;
		minute[1] = utc.second;
		minute[2] = utc.day;
		minute[3] = utc.mon;
		memcpy(&data->at(20), minute.data(), sizeof(uint8_t) * 4);
		vector<uint16_t> year(2);
		year[0] = utc.year;
		year[1] = utc.millisecond;
		memcpy(&data->at(21), year.data(), sizeof(uint16_t) * 2);
		//ʱ��
	}
	data->back() = 0xCE41CE41;	//У���
}

ReplyFrame::ReplyFrame(const ReplyFrame& copyFrom)
{
	m_startTime = time(nullptr);
	data = make_unique<vector<uint32_t>>(DATA_FRAME_SIZE, 0x5a5ac3c3);
	memcpy(data->data(), copyFrom.data->data(), 24);//����ͬ���֣����ݳ��ȣ�����id, ����˳���
	(*data)[1] = 244;
	data->back() = 0xCE41CE41;	//У���
	zsys_debug("data frame created");
}


ReplyFrame::~ReplyFrame()
{
	if (size() == DATA_FRAME_SIZE)
	{
		zsys_debug("data frame deleted");
	}
}

size_t ReplyFrame::size() const
{
	if (data == nullptr)
		return 0;
	else
		return data->size();
}

uint32_t ReplyFrame::id() const
{
	if (data == nullptr)
		return 0;
	else
		return (*data)[2];
}

NetBlock ReplyFrame::MakeBlock() const
{
	return NetBlock{ data->size() * sizeof(uint32_t), data->data()};
}

void ReplyFrame::creatgroup(int groupindex)
{
	float gpslat[SIGNAL_NUMBER];
	float gpslong[SIGNAL_NUMBER];
	float azi[SIGNAL_NUMBER];
	float ele[SIGNAL_NUMBER];
	float ampp[SIGNAL_NUMBER];
	gpslat[0] = 35.959;
	gpslat[1] = 35.5;
	gpslat[2] = 35.961;
	gpslong[0] = 120.159;
	gpslong[1] = 120.3;
	gpslong[2] = 120.161;
	for (int i = 0; i < SIGNAL_NUMBER; i++)
	{
		azi[i] = CreateRandomFloatData(81, 83);
		ele[i] = CreateRandomFloatData(-3, -1);
		ampp[i] = CreateRandomFloatData(-60, -55);
	}
	float height = 1000;

	if (m_azijo <= MIN_AZI)
	{
		m_azijo = MIN_AZI;
		deta = 1;
	}
	if (m_azijo >= MAX_AZI)
	{
		m_azijo = MAX_AZI;
		deta = -1;
	}
	m_azijo = m_azijo+ deta;
	//zsys_debug("m_azijo: %f", m_azijo);
	memcpy(&data->at(6), &height, sizeof(float));
	memcpy(&data->at(7), &gpslat[1], sizeof(float)); ///��λ [14]
	memcpy(&data->at(8), &gpslong[1], sizeof(float)); ///��λ [15]
	memcpy(&data->at(14), &azi[segDis(gen)], sizeof(float)); ///��λ [14]
	memcpy(&data->at(15), &ele[segDis(gen)], sizeof(float)); ///��λ [15]
	memcpy(&data->at(43), &m_azijo, sizeof(float)); ///��λ [43]
	memcpy(&data->at(44), &m_elejo, sizeof(float)); ///��λ [44]
	(*data)[18] = ampp[segDis(gen)]; ///
}

void ReplyFrame::randomGene(uint32_t scanIdx)
{
	creatgroup(segDis(gen));    ///20201022 �޸� ���������
	data->at(13) = 1000;       ///Ƶ��[13]
	searchNum++;
	if (searchNum % 10 == 0)
	{
		m_freqMAX = m_freqMAX + 50;
		m_freqMIN = m_freqMIN + 50;
	}
	if (searchNum > 100)
	{
		searchNum = 0;
		m_freqMAX = 200;
		m_freqMIN = 100;
	}
	vector<uint16_t> cwfreqmax{ 1,m_freqMAX };
	vector<uint16_t> freqmin{ m_freqMIN,0 };
	memcpy(&data->at(49), cwfreqmax.data(), sizeof(uint16_t) * 2);//Ƶ�����ֵ
	memcpy(&data->at(50), freqmin.data(), sizeof(uint16_t) * 2);  ///Ƶ����Сֵ
	vector<uint16_t> fre(10, 1250);
	memcpy(&data->at(23), fre.data(), sizeof(uint16_t)*10); 
	vector<uint16_t> pw(10, CreateRandomFloatData(80, 82));
	memcpy(&data->at(28), pw.data(), sizeof(uint16_t) * 10);
	vector<uint16_t> pri(10, CreateRandomFloatData(120, 122));
	memcpy(&data->at(33), pri.data(), sizeof(uint16_t) * 10);
	data->at(16) = segDis(gen); ///����
	data->at(17) = scanIdx;     ///ɨ�����к�  
	memcpy(&data->at(40), &M_GI_AZI, sizeof(float));
	data->at(19) = 20;
	//ʱ��
	auto utc = getCurrentUtc();
	vector<uint8_t> hour(4);
	hour[0] = 35;
	hour[1] = 120;
	hour[2] = 520;
	hour[3] = utc.hour;
	memcpy(&data->at(9), hour.data(), sizeof(uint8_t) * 4);
	vector<uint8_t> minute(4);
	minute[0] = utc.min;
	minute[1] = utc.second;
	minute[2] = utc.day;
	minute[3] = utc.mon;
	memcpy(&data->at(10), minute.data(), sizeof(uint8_t) * 4);
	vector<uint16_t> year(2);
	year[0] = utc.year;
	year[1] = utc.millisecond;
	memcpy(&data->at(11), year.data(), sizeof(uint16_t) * 2);
	//ʱ��
}

float ReplyFrame::CreateRandomFloatData(float minvalue, float maxvalue)
{
	static std::random_device rd;
	static std::mt19937 generate(rd());
	std::uniform_real_distribution<float> data(minvalue, maxvalue);
	return data(generate);
}

//void ReplyFrame::randomGene(uint32_t scanIdx)
//{
//	std::normal_distribution<float> aziDis(45, 0.5f);
//	std::normal_distribution<float> eleDis(30, 0.1f);
//	float azi = aziDis(gen);
//	float ele = eleDis(gen);
//	
//	memcpy(&data->at(14), &azi, sizeof(azi));
//	memcpy(&data->at(15), &ele, sizeof(ele));
//	data->at(16) = segDis(gen);
//	data->at(17) = scanIdx;
//}


//void ReplyFrame::randomGene(uint32_t scanIdx)
//{
//	std::normal_distribution<float> cazi(45, 3);
//	std::normal_distribution<float> cele(30, 5);
//	float azii = cazi(gen);
//	float elei = cele(gen);
//
//	std::normal_distribution<float> cazif(0.8, 0.1f);
//	std::normal_distribution<float> celef(0.5, 0.1f);
//	float azif = cazif(gen);
//	float elef = celef(gen);
//
//
//	std::normal_distribution<float> aziDis(azii, azif);
//	std::normal_distribution<float> eleDis(elei, elef);
//	float azi = aziDis(gen);
//	float ele = eleDis(gen);
//
//	memcpy(&data->at(14), &azi, sizeof(azi)); ///��λ
//	memcpy(&data->at(15), &ele, sizeof(ele)); ///����
//	data->at(16) = segDis(gen); ///����
//	data->at(17) = scanIdx;     ///ɨ�����к�
//}

void ReplyFrame::print() const
{
	if (data == nullptr)
		return;
	string idName("δʶ��");
	auto iter = CMD_NAME.find(id());
	if (iter != CMD_NAME.end())
	{
		idName = iter->second;
	}
	zsys_debug("�������� %s, ˳��� %d", idName.c_str(), (*data)[4]);
}

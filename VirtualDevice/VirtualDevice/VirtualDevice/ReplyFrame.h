#pragma once

class ReplyFrame
{
public:
	ReplyFrame(const ReplyFrame& copyFrom);
	ReplyFrame(const vector<byte>& cmd);
	~ReplyFrame();
public:
	size_t size() const;
	uint32_t id() const;
	float CreateRandomFloatData(float minvalue, float maxvalue);
	void randomGene(uint32_t scanIdx);
	void creatgroup(int groupindex);
	NetBlock MakeBlock() const;
	void print() const;

private:
	unique_ptr<vector<uint32_t>> data;
	float m_azijo = 45;
	float m_elejo = 4;
	int searchNum=0;
	uint16_t m_freqMAX = 200;
	uint16_t m_freqMIN = 100;
	int deta = 1;
	uint64_t m_startTime;

private:
	static const size_t REPLY_FRAME_SIZE = 32;
	static const size_t DATA_FRAME_SIZE = 64;
	static const std::map<uint32_t, string> CMD_NAME;
	const float M_GI_AZI = 45;//
	const float MIN_AZI = M_GI_AZI - 30;//
	const float MAX_AZI = M_GI_AZI + 30;//
};


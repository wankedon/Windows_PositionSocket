#include "stdafx.h"
#include "PositionSocketAPI.h"
#include "PositionSocketParser.h"
#include "PositionRawFrame.h"

PositionSocketParser::PositionSocketParser()
{
}

PositionSocketParser::~PositionSocketParser()
{
}

/**
* @brief 结果数据帧解析为结构体
* @details
* @param[in] vector<uint32_t>& resultDataFrame  结果数据帧
* @return void
* @retval
*/
void PositionSocketParser::ParserDataFrame(vector<uint32_t>& resultDataFrame)
{
	//PositionRawFrame f(resultDataFrame);
	//auto result = f.getResult();
	//if (m_rcvHandler)
	//{
	//	m_rcvHandler(result, m_userPointer);
	//}
}
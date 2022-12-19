/**
* @file PositionSocketParser.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发。

* @brief PositionSocketParser类所在头文件

* @author wangkedong

* @date 2020:06:04

* @note 

* @version <1.0.0>

*/
#pragma once

/**
* @brief 解析数据帧送入回调
*/
class PositionSocketParser
{
public:
	PositionSocketParser();
	~PositionSocketParser();

public:
	void ParserDataFrame(vector<uint32_t>& resultdataframe);

private:
	function<void(void* userPointer)> m_rcvHandler;
	void* m_userPointer;
};
/**
* @file PositionSocketParser.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת����

* @brief PositionSocketParser������ͷ�ļ�

* @author wangkedong

* @date 2020:06:04

* @note 

* @version <1.0.0>

*/
#pragma once

/**
* @brief ��������֡����ص�
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
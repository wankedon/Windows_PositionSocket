#pragma once
#include <time.h>
#include <windows.h>

namespace UTC_CONVERT
{
	typedef struct UTCTime
	{
		int year;
		int mon;
		int day;
		int hour;
		int min;
		int second;
		int millisecond;
	}UTCTime;

	typedef struct TimeStamp
	{
		uint64_t seconds;
		uint32_t microseconds;
	}TimeStamp;

	uint64_t utcToMsec(const UTCTime& utc)
	{
		struct tm tm_time;
		memset(&tm_time, 0, sizeof(tm_time));
		tm_time.tm_year = utc.year - 1900;
		tm_time.tm_mon = utc.mon - 1;
		tm_time.tm_mday = utc.day;
		tm_time.tm_hour = utc.hour;
		tm_time.tm_min = utc.min;
		tm_time.tm_sec = utc.second;
		return (uint64_t)mktime(&tm_time) * 1000 + (uint64_t)utc.millisecond;
	}

	UTCTime msecToUtc(const uint64_t& milliSeconds)
	{
		UTCTime utc;
		time_t seconds = milliSeconds / 1000;
		tm* local = localtime((time_t*)&seconds);
		utc.year = local->tm_year + 1900;
		utc.mon = local->tm_mon + 1;
		utc.day = local->tm_mday;
		utc.hour = local->tm_hour;
		utc.min = local->tm_min;
		utc.second = local->tm_sec;
		utc.millisecond = milliSeconds % 1000;
		return utc;
	}

	struct timeval getTimeVal()
	{
		struct timeval tp;
		time_t clock;
		struct tm tm;
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		tm.tm_year = wtm.wYear - 1900;
		tm.tm_mon = wtm.wMonth - 1;
		tm.tm_mday = wtm.wDay;
		tm.tm_hour = wtm.wHour;
		tm.tm_min = wtm.wMinute;
		tm.tm_sec = wtm.wSecond;
		tm.tm_isdst = -1;
		clock = mktime(&tm);
		tp.tv_sec = clock;
		tp.tv_usec = wtm.wMilliseconds * 1000;
		return tp;
	}

	uint64_t getCurrentMsec()
	{
		struct timeval timeval = getTimeVal();
		return (uint64_t)timeval.tv_sec * 1000 + (uint64_t)timeval.tv_usec / 1000;
	}

	UTCTime getCurrentUtc()
	{
		return msecToUtc(getCurrentMsec());
	}

	TimeStamp getCurrentTimeStamp()
	{
		TimeStamp timeStamp;
		struct timeval timeval = getTimeVal();
		timeStamp.seconds = timeval.tv_sec;
		timeStamp.microseconds = timeval.tv_usec;
		return timeStamp;
	}
}
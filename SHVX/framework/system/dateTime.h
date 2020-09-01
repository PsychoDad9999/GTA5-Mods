// ----------------------------------------------------------------------------

#pragma once

#include <windows.h>

// ----------------------------------------------------------------------------

// Ingame DateTimeFormat

class DateTime
{
public:	
	int getSecond();
	int getMinute();
	int getHour();
	int getDay();
	int getMonth();
	int getYear();

public:
	bool addTime(int additionalHour, int additionalMinute, int additionalSecond, int additionalDay, int additionalMonth, int additionalYear);

public:
	UINT32 getRawData();
	bool isValidTime();	
	bool isNewerThan(DateTime dateTime);
	void getTimeDifference(DateTime dateTime, int* pDiffSecond, int* pDiffMinute, int* pDiffHour, int* pDiffDay, int* pDiffMonth, int* pDiffYear);


public:
	static const UINT32 TIME_NOT_SET = 0xFFFFFFF1;


private:
	int getSecond(UINT32 rawData);
	int getMinute(UINT32 rawData);
	int getHour(UINT32 rawData);
	int getDay(UINT32 rawData);
	int getMonth(UINT32 rawData);
	int getYear(UINT32 rawData);

private:
	void setYear(UINT32* pRawData, int year);
	void setMonth(UINT32* pRawData, int month);
	void setDay(UINT32* pRawData, int day);
	void setHour(UINT32* pRawData, int hour);
	void setMinute(UINT32* pRawData, int minute);
	void setSecond(UINT32* pRawData, int second);

private:
	bool isValidTime(UINT32 rawData);
	bool isFirstTimeNewer(UINT32 rawData1, UINT32 rawData2);
	void getTimeDifference(UINT32 rawdata1, UINT32 rawdata2, int* pDiffSecond, int* pDiffMinute, int* pDiffHour, int* pDiffDay, int* pDiffMonth, int* pDiffYear);

private:
	float getClosestValue(float value, float min, float max);

private:
	int getDaysOfMonth(int month, int year);

private:
	UINT32 m_rawData;
	
public:
	DateTime(int hour, int minute, int second, int day, int month, int year);
	DateTime(UINT32 rawData);
};

// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------

#pragma once

#include "framework/system/dateTime.h"

// ----------------------------------------------------------------------------

class TimeHelper
{
public:
	static DateTime getIngameTime();
	static bool isFirstLater(DateTime time1, DateTime time2);
	static int getTimeSpanInSeconds(DateTime time1, DateTime time2);

public:
	static const UINT32 TIME_NOT_SET = 0xFFFFFFF1;

private:
	TimeHelper() {}

};

// ----------------------------------------------------------------------------
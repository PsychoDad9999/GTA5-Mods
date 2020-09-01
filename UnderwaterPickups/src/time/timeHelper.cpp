// ----------------------------------------------------------------------------

#include "timeHelper.h"

#include "inc/natives.h"

#include <ctime>

// ----------------------------------------------------------------------------

DateTime TimeHelper::getIngameTime()
{
	return DateTime(
		TIME::GET_CLOCK_HOURS(),
		TIME::GET_CLOCK_MINUTES(),
		TIME::GET_CLOCK_SECONDS(),
		TIME::GET_CLOCK_DAY_OF_MONTH(),
		TIME::GET_CLOCK_MONTH() + 1,  // API returns 0 - 11
		TIME::GET_CLOCK_YEAR());
}

// ----------------------------------------------------------------------------


bool TimeHelper::isFirstLater(DateTime time1, DateTime time2)
{
	if (!time1.isValidTime() || !time2.isValidTime())
	{
		return true;
	}

	// get years
	int yearDateTime1 = time1.getYear();
	int yearDateTime2 = time2.getYear();

	// compare year
	if (yearDateTime1 > yearDateTime2)
	{
		return true;
	}
	else if (yearDateTime1 < yearDateTime2)
	{
		return false;
	}

	// get month
	int monthDateTime1 = time1.getMonth();
	int monthDateTime2 = time2.getMonth();

	// compare month
	if (monthDateTime1 > monthDateTime2)
	{
		return true;
	}
	else if (monthDateTime1 < monthDateTime2)
	{
		return false;
	}

	// Get days
	int dayDateTime1 = time1.getDay();
	int dayDateTime2 = time2.getDay();

	// compare day
	if (dayDateTime1 > dayDateTime2)
	{
		return true;
	}
	else if (dayDateTime1 < dayDateTime2)
	{
		return false;
	}

	// Get hours
	int hourDateTime1 = time1.getHour();
	int hourDateTime2 = time2.getHour();

	// compare hours
	if (hourDateTime1 > hourDateTime2)
	{
		return true;
	}
	else if (hourDateTime1 < hourDateTime2)
	{
		return false;
	}

	// Get minutes
	int minuteDateTime1 = time1.getMinute();
	int minuteDateTime2 = time2.getMinute();

	// compare minutes
	if (minuteDateTime1 > minuteDateTime2)
	{
		return true;
	}
	else if (minuteDateTime1 < minuteDateTime2)
	{
		return false;
	}

	// get seconds
	int secondsDateTime1 = time1.getSecond();
	int secondsDateTime2 = time2.getSecond();

	// compare seconds
	if (secondsDateTime1 > secondsDateTime2)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int TimeHelper::getTimeSpanInSeconds(DateTime time1, DateTime time2)
{
	if (!time1.isValidTime() || !time2.isValidTime())
	{
		return 0;
	}	

	struct std::tm a = 
	{ 
		time1.getSecond(),
		time1.getMinute(),
		time1.getHour(),
		time1.getDay(),
		time1.getMonth() - 1,
		time1.getYear() - 1900
	}; 

	struct std::tm b = 
	{ 
		time2.getSecond(),
		time2.getMinute(),
		time2.getHour(),
		time2.getDay(),
		time2.getMonth() - 1,
		time2.getYear() - 1900
	};

	std::time_t x = std::mktime(&a);
	std::time_t y = std::mktime(&b);

	if (x != (std::time_t)(-1) && y != (std::time_t)(-1))
	{
		return abs(static_cast<int>(std::difftime(y, x)));
	}

	return 0;
}
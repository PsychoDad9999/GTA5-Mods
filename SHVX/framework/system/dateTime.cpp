// ----------------------------------------------------------------------------

#include "dateTime.h"

#include <bitset>
#include <ctime>

#include <inc/natives.h>
#include "math.h"


// ----------------------------------------------------------------------------

const UINT32 BITMASK_MONTH    = std::bitset<32>("00000000000000000000000000001111").to_ulong();
const UINT32 BITMASK_DAY      = std::bitset<32>("00000000000000000000000111110000").to_ulong();
const UINT32 BITMASK_HOUR     = std::bitset<32>("00000000000000000011111000000000").to_ulong();
const UINT32 BITMASK_MINUTE   = std::bitset<32>("00000000000011111100000000000000").to_ulong();
const UINT32 BITMASK_SECOND   = std::bitset<32>("00000011111100000000000000000000").to_ulong();
const UINT32 BITMASK_YEAR     = std::bitset<32>("01111100000000000000000000000000").to_ulong();
const UINT32 BITMASK_2011_BIT = std::bitset<32>("10000000000000000000000000000000").to_ulong();

const UINT32 BIT_OFFSET_MONTH	= 0;
const UINT32 BIT_OFFSET_DAY		= 4;
const UINT32 BIT_OFFSET_HOUR	= 9;
const UINT32 BIT_OFFSET_MINUTE	= 14;
const UINT32 BIT_OFFSET_SECOND	= 20;
const UINT32 BIT_OFFSET_YEAR	= 26;

// ----------------------------------------------------------------------------

DateTime::DateTime(UINT32 rawData)
{
	m_rawData = rawData;
}

// ----------------------------------------------------------------------------
/// <summery>Date Time</summery>
/// <param name="hour">hour [0..24]</param>
/// <param name="minute">minute [0..59]</param>
/// <param name="second">second [0..59]</param>
/// <param name="day">day [1..31]</param>
/// <param name="month">month [1..12]</param>
/// <param name="year">year [1980..2042], 2043 will reset the year to 2011</param>
/// <remarks>Setting the year to 2043 will reset the year to 2011</remarks>
// ----------------------------------------------------------------------------
DateTime::DateTime(int hour, int minute, int second, int day, int month, int year)
{
	m_rawData = 0;

	setSecond(&m_rawData, second);
	setMinute(&m_rawData, minute);
	setHour(&m_rawData, hour);
	setDay(&m_rawData, day);
	setMonth(&m_rawData, month - 1); // internal month range is [0..11]
	setYear(&m_rawData, year);
}


int DateTime::getSecond()
{
	return getSecond(m_rawData);
}

int DateTime::getMinute()
{
	return getMinute(m_rawData);
}

int DateTime::getHour()
{
	return getHour(m_rawData);
}

int DateTime::getDay()
{
	return getDay(m_rawData);
}

int DateTime::getMonth()
{
	// Internal month format is [0..11]
	return getMonth(m_rawData) + 1;
}

int DateTime::getYear()
{
	return getYear(m_rawData);
}


UINT32 DateTime::getRawData()
{
	return m_rawData;
}



bool DateTime::addTime(int additionalHours, int additionalMinutes, int additionalSeconds, int additionalDays, int additionalMonth, int additionalYears)
{
	// everything is 0
	if (additionalHours == 0 && additionalMinutes == 0 && additionalSeconds == 0 && additionalDays == 0 && additionalMonth == 0 && additionalYears == 0)
		return true;

	if (additionalHours < 0 || additionalMinutes < 0 || additionalSeconds < 0 || additionalDays < 0 || additionalMonth < 0 || additionalYears < 0)
		return false;

	int year = getYear(m_rawData);
	int month = getMonth(m_rawData);
	int day = getDay(m_rawData);
	int hour = getHour(m_rawData);
	int minute = getMinute(m_rawData);
	int second = getSecond(m_rawData);

	// calculate seconds
	second += additionalSeconds;
	while (second >= 60)
	{
		additionalMinutes++;
		second = (second - 60);
	}

	// calculate minutes
	minute += additionalMinutes;
	while (minute >= 60)
	{
		additionalHours++;
		minute = (minute - 60);
	}

	// calculate hours
	hour += additionalHours;
	while (hour >= 24)
	{
		additionalDays++;
		hour = (hour - 24);
	}

	// calculate day
	day += additionalDays;
	int daysOfMonth = getDaysOfMonth(month, year);

	while (day > daysOfMonth)
	{
		month++;
		day = (day - daysOfMonth);
		if (month > 11)
		{
			year++;
			month = (month - 12);
		}
		daysOfMonth = getDaysOfMonth(month, year);
	}

	// calculate month
	month += additionalMonth;
	while (month > 11)
	{
		additionalYears++;
		month = (month - 12);
	}

	// calculate year
	year += additionalYears;

	// set new date time
	setSecond(&m_rawData, second);
	setMinute(&m_rawData, minute);
	setHour(&m_rawData, hour);
	setMonth(&m_rawData, month);
	setDay(&m_rawData, day);
	setYear(&m_rawData, year);

	return true;
}


bool DateTime::isValidTime()
{
	return isValidTime(m_rawData);	
}


// ----------------------------------------------------------------------------
/// <summery>
/// Compares the value of this instance to a specified DateTime value 
/// and indicates whether this instance is earlier than, the same as, or later 
/// than the specified DateTime value.
/// </summery>
/// <param name="dateTime">The object to compare to the current instance.</param>
/// <returns>
/// Returns less than zero if the instance is earlier than the value.
/// Returns zero if the instance is the same as the value.
/// Returns greater than zero if the instance is later than the value.
/// </returns>
// ----------------------------------------------------------------------------
int DateTime::compareTo(DateTime dateTime)
{
	return compare(m_rawData, dateTime.getRawData());
}


void DateTime::getTimeDifference(DateTime dateTime, int* pDiffSecond, int* pDiffMinute, int* pDiffHour, int* pDiffDay, int* pDiffMonth, int* pDiffYear)
{
	getTimeDifference(m_rawData, dateTime.getRawData(), pDiffSecond, pDiffMinute, pDiffHour, pDiffDay, pDiffMonth, pDiffYear);
}


DateTime DateTime::getIngameTime()
{
	return DateTime(
		TIME::GET_CLOCK_HOURS(),
		TIME::GET_CLOCK_MINUTES(),
		TIME::GET_CLOCK_SECONDS(),
		TIME::GET_CLOCK_DAY_OF_MONTH(),
		TIME::GET_CLOCK_MONTH() + 1,  // API returns 0 - 11
		TIME::GET_CLOCK_YEAR());
}


int DateTime::getTimeDifferenceInSeconds(DateTime time1, DateTime time2)
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
		return (static_cast<int>(std::difftime(y, x)));
	}

	return 0;
}


// ----------------------------------------------------------------------------
/// <summery>
/// Compares two instances of DateTime and returns an integer that indicates 
/// whether the first instance is earlier than, the same as, or later than 
/// the second instance.
/// </summery>
/// <param name="t1">The first object to compare.</param>
/// <param name="t2">The second object to compare.</param>
/// <returns>
/// Returns less than zero if the first object is earlier than the second object.
/// Returns zero if the first object is the same as the second object.
/// Returns greater than zero if the first object is later than the second object.
/// </returns>
// ----------------------------------------------------------------------------
int DateTime::compare(DateTime t1, DateTime t2)
{
	return t1.compareTo(t2);
}


int DateTime::getSecond(UINT32 rawData)
{
	return (rawData & BITMASK_SECOND) >> BIT_OFFSET_SECOND;
}

int DateTime::getMinute(UINT32 rawData)
{
	return (rawData & BITMASK_MINUTE) >> BIT_OFFSET_MINUTE;
}

int DateTime::getHour(UINT32 rawData)
{
	return (rawData & BITMASK_HOUR) >> BIT_OFFSET_HOUR;
}

int DateTime::getDay(UINT32 rawData)
{
	return (rawData & BITMASK_DAY) >> BIT_OFFSET_DAY;
}

int DateTime::getMonth(UINT32 rawData)
{
	return (rawData & BITMASK_MONTH) >> BIT_OFFSET_MONTH;
}

int DateTime::getYear(UINT32 rawData)
{
	int bitValue = ((rawData & BITMASK_2011_BIT) != 0) ? -1 : 1;

	return ((rawData >> BIT_OFFSET_YEAR) & 31) * bitValue + 2011;
}


int DateTime::getDaysOfMonth(int month, int year)
{
	if (year < 0)
		year = 0;

	switch (month)
	{
	case 0:
	case 2:
	case 4:
	case 6:
	case 7:
	case 9:
	case 11:
		return 31;

	case 3:
	case 5:
	case 8:
	case 10:
		return 30;

	case 1:
		if ((year % 4) == 0)
		{
			if ((year % 100) != 0)
			{
				return 29;
			}
			else if ((year % 400) == 0)
			{
				return 29;
			}
		}
		return 28;

	default:
		return 30;
	}	
}



void DateTime::setYear(UINT32* pRawData, int year)
{
	if (year < 1979 || year > 2043)
		return;

	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_YEAR);

	if (year < 2011)
	{
		// set new value
		*pRawData = *pRawData | ((2011 - year) << BIT_OFFSET_YEAR);
		
		// Set bit if year is smaller than 2011
		*pRawData |= BITMASK_2011_BIT;
	}
	else
	{
		// set new value
		*pRawData = *pRawData | ((year - 2011) << BIT_OFFSET_YEAR);

		// clear 2011 bit
		*pRawData = *pRawData - (*pRawData & BITMASK_2011_BIT);
	}
}


void DateTime::setMonth(UINT32* pRawData, int month)
{
	if (month < 0 || month > 11)
		return;

	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_MONTH);

	// set new value
	*pRawData = *pRawData | (month << BIT_OFFSET_MONTH);
}



void DateTime::setDay(UINT32* pRawData, int day)
{
	int month;
	int year;

	month = getMonth(*pRawData);
	year = getYear(*pRawData);

	if (day < 1 || day > getDaysOfMonth(month, year))
		return;
	
	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_DAY);

	// set new value
	*pRawData = *pRawData | (day << BIT_OFFSET_DAY);
}



void DateTime::setHour(UINT32* pRawData, int hour)
{
	if (hour < 0 || hour > 24)
		return;

	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_HOUR);

	// set new value
	*pRawData = *pRawData | (hour << BIT_OFFSET_HOUR);
}


void DateTime::setMinute(UINT32* pRawData, int minute)
{
	if (minute < 0 || minute >= 60)
		return;
	
	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_MINUTE);

	// set new value
	*pRawData = *pRawData | (minute << BIT_OFFSET_MINUTE);
}


void DateTime::setSecond(UINT32* pRawData, int second)
{
	if (second < 0 || second >= 60)
		return;
	
	// clear old value
	*pRawData = *pRawData - (*pRawData & BITMASK_SECOND);

	// set new value
	*pRawData = *pRawData | (second << BIT_OFFSET_SECOND);
}



bool DateTime::isValidTime(UINT32 rawData)
{
	int seconds;
	int minutes;
	int hours;
	int year;
	int month;
	int day;

	if (rawData == TIME_NOT_SET)
	{
		return false;
	}

	seconds = getSecond(rawData);
	if (seconds < 0 || seconds >= 60)
	{
		return false;
	}

	minutes = getMinute(rawData);
	if (minutes < 0 || minutes >= 60)
	{
		return false;
	}

	hours = getHour(rawData);
	if (hours < 0 || hours > 23)
	{
		return false;
	}

	year = getYear(rawData);
	if (year < 1979 || year > 2043)
	{
		return false;
	}

	month = getMonth(rawData);
	if (month < 0 || month > 11)
	{
		return false;
	}

	day = getDay(rawData);
	if (day < 1 || day > getDaysOfMonth(month, year))
	{
		return false;
	}

	return true;
}



// ----------------------------------------------------------------------------
/// <summery>
/// Compares two instances of DateTime RawData and returns an integer that indicates 
/// whether the first instance is earlier than, the same as, or later than 
/// the second instance.
/// </summery>
/// <param name="rawData1">The first object to compare.</param>
/// <param name="rawData2">The second object to compare.</param>
/// <returns>
/// Returns less than zero if the first object is earlier than the second object.
/// Returns zero if the first object is the same as the second object.
/// Returns greater than zero if the first object is later than the second object.
/// </returns>
// ----------------------------------------------------------------------------
int DateTime::compare(UINT32 rawData1, UINT32 rawData2)
{
	if (rawData1 == rawData2)
		return 0;

	if (!isValidTime(rawData1))
	{
		return -1;
	}

	if (!isValidTime(rawData2))
	{
		return 1;
	}	
	
	// get years
	int yearDateTime1 = getYear(rawData1);
	int yearDateTime2 = getYear(rawData2);

	// compare year
	if (yearDateTime1 > yearDateTime2)
	{
		return 1;
	}
	else if (yearDateTime1 < yearDateTime2)
	{
		return -1;
	}

	// get month
	int monthDateTime1 = getMonth(rawData1);
	int monthDateTime2 = getMonth(rawData2);

	// compare month
	if (monthDateTime1 > monthDateTime2)
	{
		return 1;
	}
	else if (monthDateTime1 < monthDateTime2)
	{
		return -1;
	}

	// Get days
	int dayDateTime1 = getDay(rawData1);
	int dayDateTime2 = getDay(rawData2);

	// compare day
	if (dayDateTime1 > dayDateTime2)
	{
		return 1;
	}
	else if (dayDateTime1 < dayDateTime2)
	{
		return -1;
	}

	// Get hours
	int hourDateTime1 = getHour(rawData1);
	int hourDateTime2 = getHour(rawData2);

	// compare hours
	if (hourDateTime1 > hourDateTime2)
	{
		return 1;
	}
	else if (hourDateTime1 < hourDateTime2)
	{
		return -1;
	}

	// Get minutes
	int minuteDateTime1 = getMinute(rawData1);
	int minuteDateTime2 = getMinute(rawData2);

	// compare minutes
	if (minuteDateTime1 > minuteDateTime2)
	{
		return 1;
	}
	else if (minuteDateTime1 < minuteDateTime2)
	{
		return -1;
	}

	// get seconds
	int secondsDateTime1 = getSecond(rawData1);
	int secondsDateTime2 = getSecond(rawData2);

	// compare seconds
	if (secondsDateTime1 > secondsDateTime2)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


void DateTime::getTimeDifference(UINT32 rawdata1, UINT32 rawdata2, int* pDiffSecond, int* pDiffMinute, int* pDiffHour, int* pDiffDay, int* pDiffMonth, int* pDiffYear)
{
	int month;
	int year;
	int diffYear;
	int diffMonth;
	int diffDays;
	int diffHours;
	int diffMinutes;
	int diffSeconds;

	if (compare(rawdata1, rawdata2) > 0)
	{
		month = getMonth(rawdata2);
		year = getYear(rawdata1);
		diffYear = (getYear(rawdata1) - getYear(rawdata2));
		diffMonth = (getMonth(rawdata1) - getMonth(rawdata2));
		diffDays = (getDay(rawdata1) - getDay(rawdata2));
		diffHours = (getHour(rawdata1) - getHour(rawdata2));
		diffMinutes = (getMinute(rawdata1) - getMinute(rawdata2));
		diffSeconds = (getSecond(rawdata1) - getSecond(rawdata2));
	}
	else
	{
		month = getMonth(rawdata1);
		year = getYear(rawdata2);
		diffYear = (getYear(rawdata2) - getYear(rawdata1));
		diffMonth = (getMonth(rawdata2) - getMonth(rawdata1));
		diffDays = (getDay(rawdata2) - getDay(rawdata1));
		diffHours = (getHour(rawdata2) - getHour(rawdata1));
		diffMinutes = (getMinute(rawdata2) - getMinute(rawdata1));
		diffSeconds = (getSecond(rawdata2) - getSecond(rawdata1));
	}

	while (diffSeconds < 0)
	{
		diffSeconds += 60;
		diffMinutes = (diffMinutes - 1);
	}

	while (diffSeconds > 59)
	{
		diffSeconds = (diffSeconds - 60);
		diffMinutes++;
	}

	while (diffMinutes < 0)
	{
		diffMinutes += 60;
		diffHours = (diffHours - 1);
	}

	while (diffMinutes > 59)
	{
		diffMinutes = (diffMinutes - 60);
		diffHours++;
	}

	while (diffHours < 0)
	{
		diffHours += 24;
		diffDays = (diffDays - 1);
	}

	while (diffHours > 23)
	{
		diffHours = (diffHours - 24);
		diffDays++;
	}

	while (diffDays < 0)
	{
		while (diffMonth < 0)
		{
			diffMonth += 12;
			diffYear = (diffYear - 1);
		}
		diffDays = (diffDays + getDaysOfMonth(month, year));
		diffMonth = (diffMonth - 1);
		month = static_cast<int>(round(Math::getNearestValue(static_cast<float>(month + 1), 0.f, 12.f)));
	}

	while (diffMonth < 0)
	{
		diffMonth += 12;
		diffYear = (diffYear - 1);
	}

	while (diffMonth > 12)
	{
		diffMonth = (diffMonth - 12);
		diffYear++;
	}

	*pDiffSecond = diffSeconds;
	*pDiffMinute = diffMinutes;
	*pDiffHour = diffHours;
	*pDiffDay = diffDays;
	*pDiffMonth = diffMonth;
	*pDiffYear = diffYear;
}
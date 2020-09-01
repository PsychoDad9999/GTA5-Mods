// ----------------------------------------------------------------------------

#include "typeConverter.h"

#include <stdexcept>

#include "stringUtils.h"

// ----------------------------------------------------------------------------

bool TypeConverter::intToBoolSafe(int value)
{
	return value == 1;
}


ULONG TypeConverter::intToULongSafe(int value, ULONG minValue, ULONG maxValue)
{
	return intToULongSafe(value, minValue, maxValue, 0, false);	
}


ULONG TypeConverter::intToULongSafe(int value, ULONG minValue, ULONG maxValue, ULONG exceptionalValue)
{
	return intToULongSafe(value, minValue, maxValue, exceptionalValue, true);
}


ULONG TypeConverter::intToULongSafe(int value, ULONG minValue, ULONG maxValue, ULONG exceptionalValue, bool useExceptionalValue)
{
	if (maxValue < minValue)
		throw std::invalid_argument("maxvalue must be greater or equal than minValue");

	ULONG uValue = static_cast<ULONG>(value);

	if (useExceptionalValue && uValue == exceptionalValue) // e.g. allow zero
		return uValue;

	if (uValue < minValue)
		uValue = minValue;

	if (uValue > maxValue)
		uValue = maxValue;

	return uValue;
}


std::wstring TypeConverter::strToKeyNameSafe(const wchar_t* str)
{
	std::wstring ws(str);

	// trim comment
	ws = StringUtils::rtrimComment(ws);

	// trim left
	ws = StringUtils::ltrim(ws);

	// trim right
	ws = StringUtils::rtrim(ws);

	// convert to lowercase
	StringUtils::toLower(ws);

	return ws;
}


// ----------------------------------------------------------------------------


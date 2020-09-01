// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <string>

// ----------------------------------------------------------------------------

class TypeConverter
{
public:
	static bool intToBoolSafe(int value);
	static ULONG intToULongSafe(int value, ULONG minValue, ULONG maxValue);
	static ULONG intToULongSafe(int value, ULONG minValue, ULONG maxValue, ULONG exceptionalValue);

	static std::wstring strToKeyNameSafe(const wchar_t* str);

private:
	static ULONG intToULongSafe(int value, ULONG minValue, ULONG maxValue, ULONG exceptionalValue, bool useExceptionalValue);

private:
	TypeConverter() {}
};
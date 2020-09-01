// ----------------------------------------------------------------------------

#include "stringUtils.h"

#include <Windows.h>
#include <algorithm>
#include <cwctype>

// ----------------------------------------------------------------------------

const std::wstring StringUtils::WHITESPACE = TEXT(" \n\r\t\f\v");
const std::wstring StringUtils::COMMENT = TEXT(";");

// ----------------------------------------------------------------------------

void StringUtils::toLower(wchar_t* buffer)
{
	for (int i = 0; i < wcslen(buffer); i++)
		putwchar(towlower(buffer[i]));
}


void StringUtils::toLower(std::wstring& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::wstring StringUtils::ltrim(const std::wstring& str)
{
	size_t start = str.find_first_not_of(WHITESPACE);

	if (start == std::wstring::npos)
		return TEXT("");
	else
		return str.substr(start);
}


std::wstring StringUtils::rtrim(const std::wstring& str)
{
	size_t start = str.find_first_of(WHITESPACE);

	if (start == std::wstring::npos)
		return str;
	else
		return str.substr(0, start);
}


std::wstring StringUtils::rtrimComment(const std::wstring& str)
{
	size_t first = str.find_first_of(COMMENT);

	if (first == std::wstring::npos) // No comment found
		return str;
	else if (first == 0)
		return TEXT("");
	else
		return str.substr(0, first);
}



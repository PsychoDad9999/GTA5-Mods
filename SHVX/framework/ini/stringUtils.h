// ----------------------------------------------------------------------------

#pragma once

#include <string>

// ----------------------------------------------------------------------------

class StringUtils
{
public:
	static void toLower(wchar_t* buffer);
	static void toLower(std::wstring& str);

	static std::wstring ltrim(const std::wstring& str);
	static std::wstring rtrim(const std::wstring& str);
	static std::wstring rtrimComment(const std::wstring& str);

private:
	static const std::wstring WHITESPACE;
	static const std::wstring COMMENT;

private:
	StringUtils() {}
};

// ----------------------------------------------------------------------------
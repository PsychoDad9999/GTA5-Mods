// ----------------------------------------------------------------------------

#pragma once

#include <string>

// ----------------------------------------------------------------------------

class FileHelper
{
public:
	static bool fileExists(const std::wstring& filename);

private:
	FileHelper() {}
};

// ----------------------------------------------------------------------------
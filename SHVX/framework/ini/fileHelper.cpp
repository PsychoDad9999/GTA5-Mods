// ----------------------------------------------------------------------------

#include "fileHelper.h"

#include <fstream>

// ----------------------------------------------------------------------------

bool FileHelper::fileExists(const std::wstring& filename)
{
	std::ifstream ifile(filename.c_str());

	return ifile.good();
}

// ----------------------------------------------------------------------------
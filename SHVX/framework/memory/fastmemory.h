// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

class FastMemory
{
public:
	static UINT64* findPattern(const char* pattern, const char* mask);

	static bool readInt32FromMemory(UINT64* address, int* value, bool readFromLower32Bits);
	static bool readFloatFromMemory(UINT64* address, float* value, bool readFromLower32Bits);

	static bool writeInt32ToMemory(UINT64* address, int value, bool writeToLower32Bits);
	static bool writeFloatToMemory(UINT64* address, float value, bool writeToLower32Bits);


private:
	FastMemory() {}
};

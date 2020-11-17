// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

class Memory
{
public:
	enum ScanAlignment : int
	{
		Byte = 1,  // 8 Bit alignment, slowest scan method
		DWord = 4, // 32 Bit alignment
		QWord = 8, // 64 Bit alignment, fastest scan method
	};

public:
	static UINT64* findPattern(const char* pattern, const char* mask);
	static UINT64* findPattern(const char* pattern, const char* mask, ScanAlignment alignment);
	static UINT64* findPattern(const char* pattern, const char* mask, BYTE* startAddress, ScanAlignment alignment);
	static UINT64* findPattern(const char* pattern, const char* mask, BYTE* startAddress, BYTE* endAddress, ScanAlignment alignment);

	static bool readInt32FromMemory(UINT64* address, int* value, bool readFromLowerDWord);
	static bool readInt64FromMemory(UINT64* address, INT64* value);
	static bool readFloatFromMemory(UINT64* address, float* value, bool readFromLowerDWord);

	static bool writeInt32ToMemory(UINT64* address, int value, bool writeToLowerDWord);
	static bool writeFloatToMemory(UINT64* address, float value, bool writeToLowerDWord);

private:
	static const BYTE* scanMemory(const char* pattern, const char* mask, const BYTE* pAddress, const SIZE_T size, ScanAlignment alignment);

private:
	Memory() {}
};

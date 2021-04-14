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

	enum PageType :int
	{
		Any = 0,
		Image = MEM_IMAGE,
		Mapped = MEM_MAPPED,
		Private = MEM_PRIVATE,
	};

public:
	static UINT64* findPattern(const char* pattern, const char* mask);
	static UINT64* findPattern(const char* pattern, const char* mask, ScanAlignment alignment, PageType type, bool directAccess);
	static UINT64* findPattern(const char* pattern, const char* mask, BYTE* startAddress, ScanAlignment alignment, PageType type, bool directAccess);
	static UINT64* findPattern(const char* pattern, const char* mask, BYTE* startAddress, BYTE* endAddress, ScanAlignment alignment, PageType type, bool directAccess);

	static bool readInt32FromMemory(UINT64* address, int* value, bool readFromLowerDWord, bool directAccess);
	static bool readInt64FromMemory(UINT64* address, INT64* value, bool directAccess);
	static bool readFloatFromMemory(UINT64* address, float* value, bool readFromLowerDWord, bool directAccess);

	static bool writeInt32ToMemory(UINT64* address, int value, bool writeToLowerDWord, bool directAccess);
	static bool writeFloatToMemory(UINT64* address, float value, bool writeToLowerDWord, bool directAccess);

private:
	static const BYTE* scanMemory(const char* pattern, const char* mask, const BYTE* pAddress, const SIZE_T size, ScanAlignment alignment);

private:
	Memory() {}
};

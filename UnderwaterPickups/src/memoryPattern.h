// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <framework/memory/memory.h>

// ----------------------------------------------------------------------------

class MemoryPattern
{
public:
	static UINT64* getPickupSpawnTimeGlobal();

private:
	typedef bool (*funcIsValid)(UINT64**, bool);

private:	
	static UINT64* getValidPatternMatch(const char* pattern, const char* mask, Memory::ScanAlignment alignment, Memory::PageType type, bool directAccess, funcIsValid pIsValid);
	static bool isValidMatch(UINT64** pMatch, bool directAccess);

private:
	MemoryPattern() {}
};


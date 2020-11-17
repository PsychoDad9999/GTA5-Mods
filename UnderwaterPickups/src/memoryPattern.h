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
	typedef bool (*funcIsValid)(UINT64*);

private:	
	static UINT64* getValidPatternMatch(const char* pattern, const char* mask, Memory::ScanAlignment alignment, funcIsValid pIsValid);
	static bool isValidMatch(UINT64* pMatch);

private:
	MemoryPattern() {}
};


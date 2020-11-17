// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

class MemorySearch
{
public:
	static UINT64* getPickupSpawnTimeGlobal();

private:
	static UINT64* findMemoryMatch();
	static bool isValidMatch(UINT64* pMatch);


private:
	MemorySearch() {}
};


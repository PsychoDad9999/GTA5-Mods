// ----------------------------------------------------------------------------

#include "memorysearch.h"

#include <framework/memory/memory.h>
#include <framework/system/dateTime.h>

// ----------------------------------------------------------------------------

UINT64* MemorySearch::findMemoryMatch()
{
	const char* SPAWNTIME_PATTERN = "\x19\xFC\xFF\xFF\x00\x00\x00\x00";  // QWORD Pattern
	const char* SPAWNTIME_MASK = "xxxxxxxx";

	Memory::ScanAlignment alignment = Memory::ScanAlignment::QWord; // fastest scan method

	UINT64* pMatch = Memory::findPattern(SPAWNTIME_PATTERN, SPAWNTIME_MASK, alignment);

	if (pMatch == nullptr || isValidMatch(pMatch))
		return pMatch;

	while (true)
	{
		// there are multiple matches with the same pattern, so search again until the match is valid
		pMatch = Memory::findPattern(SPAWNTIME_PATTERN, SPAWNTIME_MASK, reinterpret_cast<BYTE*>(pMatch + 1), alignment);

		if (pMatch == nullptr || isValidMatch(pMatch))
			return pMatch;
	};
}


bool MemorySearch::isValidMatch(UINT64* pMatch)
{
	INT64 value = 0;

	// check if previous DWord is zero
	if (Memory::readInt64FromMemory(pMatch - 1, &value))
	{
		return value == 0;
	}

	return false;
}



UINT64* MemorySearch::getPickupSpawnTimeGlobal()
{
	UINT64* pMatch = findMemoryMatch();

	if (pMatch != nullptr)
	{
		// Pointer offset
		pMatch -= 107;

		INT32 value = 0;

		// make another consistency check, to make sure we are reading the correct memory address
		if (Memory::readInt32FromMemory(pMatch, &value, true))
		{
			if (value == DateTime::TIME_NOT_SET)
				return pMatch;
		}		
	}

	return nullptr;
}




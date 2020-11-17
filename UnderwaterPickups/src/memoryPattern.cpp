// ----------------------------------------------------------------------------

#include "memoryPattern.h"

#include <framework/system/dateTime.h>

// ----------------------------------------------------------------------------

UINT64* MemoryPattern::getPickupSpawnTimeGlobal()
{
	const char* SPAWNTIME_PATTERN = "\x19\xFC\xFF\xFF\x00\x00\x00\x00";  // QWORD Pattern
	const char* SPAWNTIME_MASK = "xxxxxxxx";

	UINT64* pMatch = getValidPatternMatch(SPAWNTIME_PATTERN, SPAWNTIME_MASK, Memory::ScanAlignment::QWord, &isValidMatch);

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


bool MemoryPattern::isValidMatch(UINT64* pMatch)
{
	INT64 value = 0;

	// check if previous DWord is zero
	if (Memory::readInt64FromMemory(pMatch - 1, &value))
	{
		return value == 0;
	}

	return false;
}


UINT64* MemoryPattern::getValidPatternMatch(const char* pattern, const char* mask, Memory::ScanAlignment alignment, funcIsValid pFuncIsValid)
{
	if (pFuncIsValid == nullptr)
		return nullptr;

	UINT64* pMatch = Memory::findPattern(pattern, mask, alignment);

	if (pMatch == nullptr || pFuncIsValid(pMatch))
		return pMatch;

	while (true)
	{
		// there are multiple matches with the same pattern, so search again until the match is valid
		pMatch = Memory::findPattern(pattern, mask, reinterpret_cast<BYTE*>(pMatch + 1), alignment);

		if (pMatch == nullptr || pFuncIsValid(pMatch))
			return pMatch;
	};
}


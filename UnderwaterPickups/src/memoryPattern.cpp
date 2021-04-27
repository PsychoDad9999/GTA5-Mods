// ----------------------------------------------------------------------------

#include "memoryPattern.h"

#include <stdio.h>
#include <framework/system/dateTime.h>

// ----------------------------------------------------------------------------

UINT64* MemoryPattern::getPickupSpawnTimeGlobal()
{
	const char* SPAWNTIME_PATTERN = "\x19\xFC\xFF\xFF\x00\x00\x00\x00";  // QWORD Pattern
	const char* SPAWNTIME_MASK = "xxxxxxxx";

	UINT64* pMatch = getValidPatternMatch(SPAWNTIME_PATTERN, SPAWNTIME_MASK, Memory::ScanAlignment::QWord, Memory::PageType::Private, true, &isValidMatch);

	return pMatch;
}


bool MemoryPattern::isValidMatch(UINT64** pMatch, bool directAccess)
{
	INT64 value = 0;

	// check if previous DWord is zero
	if (Memory::readInt64FromMemory(*pMatch - 1, &value, directAccess))
	{
		if (value == 0)
		{
			// Pointer offset
			UINT64* pMatchWithOffset = *pMatch - 107;
			INT32 value = 0;

			// make another consistency check, to make sure we are reading the correct memory address
			if (Memory::readInt32FromMemory(pMatchWithOffset, &value, true, directAccess))
			{
				// check if raw data is a valid date time object
				if (value == DateTime::TIME_NOT_SET || DateTime(value).isValidTime())
				{
					*pMatch = pMatchWithOffset;
					return true;
				}
			}
		}
	}

	return false;
}


UINT64* MemoryPattern::getValidPatternMatch(const char* pattern, const char* mask, Memory::ScanAlignment alignment, Memory::PageType type, bool directAccess, funcIsValid pFuncIsValid)
{
	if (pFuncIsValid == nullptr)
		return nullptr;

	UINT64* pMatch = Memory::findPattern(pattern, mask, alignment, type, directAccess);

	if (pMatch == nullptr || pFuncIsValid(&pMatch, directAccess))
		return pMatch;

	while (true)
	{
		// there are multiple matches with the same pattern, so search again until the match is valid
		pMatch = Memory::findPattern(pattern, mask, reinterpret_cast<BYTE*>(pMatch + 1), alignment, type, directAccess);

		if (pMatch == nullptr || pFuncIsValid(&pMatch, directAccess))
			return pMatch;
	};
}


// ----------------------------------------------------------------------------

#include "fastmemory.h"

#include "framework/system/module.h"

#include <string>
#include <Psapi.h>

// ----------------------------------------------------------------------------

UINT64* FastMemory::findPattern(const char* pattern, const char* mask)
{
	SIZE_T patternLength = strlen(pattern);
	SIZE_T maskLength = strlen(mask);

	if (patternLength != maskLength)
		return nullptr;

	MODULEINFO moduleInfo;
	memset(&moduleInfo, 0, sizeof(moduleInfo));

	if (GetModuleInformation(GetCurrentProcess(), Module::getModule(), &moduleInfo, sizeof(moduleInfo)))
	{		 
		UINT64 baseAddress = reinterpret_cast<UINT64>(moduleInfo.lpBaseOfDll);
		UINT64 endAddress = baseAddress + moduleInfo.SizeOfImage;

		UINT64 address = baseAddress;

		for (; address < endAddress; address++)
		{
			for (int i = 0; i < patternLength; i++)
			{
				if (mask[i] != '?' && ((char*)address)[i] != pattern[i])
				{					
					break;
				}
				else if (i + 1 == patternLength)
				{
					return reinterpret_cast<UINT64*>(address);
				}
			}
		}

		return nullptr;
	}
}


bool FastMemory::readInt32FromMemory(UINT64* address, int* value, bool readFromLower32Bits)
{
	bool succeed = false;
	*value = 0;

	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!readFromLower32Bits)
	{
		pAddress += 4;
	}

	MODULEINFO moduleInfo;
	memset(&moduleInfo, 0, sizeof(moduleInfo));

	if (GetModuleInformation(GetCurrentProcess(), Module::getModule(), &moduleInfo, sizeof(moduleInfo)))
	{
		BYTE* baseAddress = reinterpret_cast<BYTE*>(moduleInfo.lpBaseOfDll);
		BYTE* endAddress = baseAddress + moduleInfo.SizeOfImage;

		if (pAddress >= baseAddress && pAddress <= endAddress)
		{
			memcpy(value, pAddress, 4);
			return true;
		}
	}

	return false;
}


bool FastMemory::readFloatFromMemory(UINT64* address, float* value, bool readFromLower32Bits)
{
	*value = 0.0f;
	int intValue = 0;

	if (readInt32FromMemory(address, &intValue, readFromLower32Bits))
	{
		*value = *reinterpret_cast<float*>(&intValue);
		return true;
	}

	return false;
}

bool FastMemory::writeInt32ToMemory(UINT64* address, int value, bool writeToLower32Bits)
{
	bool succeed = false;

	// convert 64-Bit Pointer 1 Byte Pointer
	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!writeToLower32Bits)
	{
		// write to upper 4 bytes of address
		pAddress += 4;
	}

	MODULEINFO moduleInfo;
	memset(&moduleInfo, 0, sizeof(moduleInfo));

	if (GetModuleInformation(GetCurrentProcess(), Module::getModule(), &moduleInfo, sizeof(moduleInfo)))
	{
		BYTE* baseAddress = reinterpret_cast<BYTE*>(moduleInfo.lpBaseOfDll);
		BYTE* endAddress = baseAddress + moduleInfo.SizeOfImage;

		if (pAddress >= baseAddress && pAddress <= endAddress)
		{
			memcpy(pAddress, &value, 4);

			return true;
		}
	}

	return false;
}


bool FastMemory::writeFloatToMemory(UINT64* address, float value, bool writeToLower32Bits)
{
	int intValue = *reinterpret_cast<int*>(&value);

	return writeInt32ToMemory(address, intValue, writeToLower32Bits);
}
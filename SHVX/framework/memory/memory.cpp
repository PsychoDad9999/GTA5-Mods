// ----------------------------------------------------------------------------

#include "memory.h"

#include <string>

// ----------------------------------------------------------------------------


BYTE* Memory::scan(char* pattern, char* mask, BYTE* begin, SIZE_T size)
{
	SIZE_T patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != static_cast<char>(*(begin + i + j)))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return (begin + i);
		}
	}

	return nullptr;
}


UINT64* Memory::findPattern(char* pattern, char* mask)
{
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);

	__int64 end = (__int64)sysInfo.lpMaximumApplicationAddress;

	BYTE* currentChunk = NULL;
	BYTE* match = nullptr;
	SIZE_T bytesRead;

	while (currentChunk < (BYTE*)end)
	{
		MEMORY_BASIC_INFORMATION mbi;

		HANDLE process = GetCurrentProcess();
		int hr = GetLastError();

		if (!VirtualQueryEx(process, currentChunk, &mbi, sizeof(mbi)))
		{
			return nullptr;
		}

		BYTE* buffer = nullptr;

		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
		{
			buffer = new BYTE[mbi.RegionSize];
			DWORD oldprotect;

			if (VirtualProtectEx(process, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
			{
				ReadProcessMemory(process, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
				VirtualProtectEx(process, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);

				BYTE* internalAddress = scan(pattern, mask, buffer, bytesRead);

				if (internalAddress != nullptr)
				{
					//calculate from internal to external

					__int64 offsetFromBuffer = internalAddress - buffer;
					match = currentChunk + offsetFromBuffer;
					delete[] buffer;
					break;
				}
			}
		}

		currentChunk = currentChunk + mbi.RegionSize;

		if (buffer)
			delete[] buffer;

		buffer = nullptr;
	}

	return reinterpret_cast<UINT64*>(match);
}


bool Memory::readInt32FromMemory(UINT64* address, int* value, bool readFromLower32Bits)
{
	bool succeed = false;
	*value = 0;

	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!readFromLower32Bits)
	{
		pAddress += 4;
	}

	MEMORY_BASIC_INFORMATION mbi;

	const SIZE_T READ_SIZE = 4; //4 Bytes

	HANDLE process = GetCurrentProcess();
	if (VirtualQueryEx(process, pAddress, &mbi, sizeof(mbi)))
	{
		if (READ_SIZE <= mbi.RegionSize)
		{
			if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
			{
				BYTE* buffer = new BYTE[READ_SIZE];
				DWORD oldprotect;

				if (VirtualProtectEx(process, pAddress, READ_SIZE, PAGE_EXECUTE_READWRITE, &oldprotect))
				{
					ReadProcessMemory(process, pAddress, buffer, READ_SIZE, NULL);
					VirtualProtectEx(process, pAddress, READ_SIZE, oldprotect, &oldprotect);

					memcpy(value, buffer, 4);

					succeed = true;
				}

				delete[] buffer;
			}
		}
	}

	return succeed;
}


bool Memory::readFloatFromMemory(UINT64* address, float* value, bool readFromLower32Bits)
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

bool Memory::writeInt32ToMemory(UINT64* address, int value, bool writeToLower32Bits)
{
	bool succeed = false;

	// convert 64-Bit Pointer 1 Byte Pointer
	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!writeToLower32Bits)
	{
		// write to upper 4 bytes of address
		pAddress += 4;
	}

	MEMORY_BASIC_INFORMATION mbi;

	const SIZE_T WRITE_SIZE = 4; //4 Bytes
	SIZE_T bytesWritten = 0;

	HANDLE process = GetCurrentProcess();
	if (VirtualQueryEx(process, pAddress, &mbi, sizeof(mbi)))
	{
		if (WRITE_SIZE <= mbi.RegionSize)
		{
			if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
			{
				BYTE* buffer = new BYTE[WRITE_SIZE];
				DWORD oldprotect;

				if (VirtualProtectEx(process, pAddress, WRITE_SIZE, PAGE_EXECUTE_READWRITE, &oldprotect))
				{
					memcpy(buffer, &value, WRITE_SIZE);

					WriteProcessMemory(process, pAddress, buffer, WRITE_SIZE, &bytesWritten);
					VirtualProtectEx(process, pAddress, WRITE_SIZE, oldprotect, &oldprotect);

					succeed = bytesWritten == WRITE_SIZE;;
				}

				delete[] buffer;
			}
		}
	}

	return succeed;
}


bool Memory::writeFloatToMemory(UINT64* address, float value, bool writeToLower32Bits)
{
	int intValue = *reinterpret_cast<int*>(&value);

	return writeInt32ToMemory(address, intValue, writeToLower32Bits);
}
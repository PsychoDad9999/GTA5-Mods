// ----------------------------------------------------------------------------

#include "memory.h"

#include <string>

// ----------------------------------------------------------------------------


UINT64* Memory::findPattern(const char* pattern, const char* mask)
{
	return findPattern(pattern, mask, ScanAlignment::Byte, PageType::Any, false);
}


UINT64* Memory::findPattern(const char* pattern, const char* mask, ScanAlignment alignment, PageType type, bool directAccess)
{
	SYSTEM_INFO sysInfo;
	memset(&sysInfo, 0, sizeof(sysInfo));

	GetSystemInfo(&sysInfo);

	return findPattern(pattern, mask, reinterpret_cast<BYTE*>(sysInfo.lpMinimumApplicationAddress), reinterpret_cast<BYTE*>(sysInfo.lpMaximumApplicationAddress), alignment, type, directAccess);
}


UINT64* Memory::findPattern(const char* pattern, const char* mask, BYTE* startAddress, ScanAlignment alignment, PageType type, bool directAccess)
{
	SYSTEM_INFO sysInfo;
	memset(&sysInfo, 0, sizeof(sysInfo));

	GetSystemInfo(&sysInfo);

	return findPattern(pattern, mask, startAddress, reinterpret_cast<BYTE*>(sysInfo.lpMaximumApplicationAddress), alignment, type, directAccess);
}


UINT64* Memory::findPattern(const char* pattern, const char* mask, BYTE* startAddress, BYTE* endAddress, ScanAlignment alignment, PageType type, bool directAccess)
{		
	BYTE* pMemChunk = startAddress;	
	SIZE_T bytesRead = 0;

	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));

	HANDLE hProcess = GetCurrentProcess();

	while (pMemChunk < endAddress)
	{		
		if (!VirtualQueryEx(hProcess, pMemChunk, &mbi, sizeof(mbi)))
		{
			return nullptr;
		}
		
		BYTE* pBuffer = nullptr;

		// calculate page region offset in case startaddress != mbi.BaseAddress
		INT64 regionOffset = pMemChunk - reinterpret_cast<BYTE*>(mbi.BaseAddress);

		// calculate page region size
		SIZE_T regionSize = mbi.RegionSize - regionOffset;

		// validate region size
		if (regionOffset < 0 || regionSize <= 0)
			return nullptr;

		// skip page if type doesn't match
		if (type == PageType::Any || mbi.Type == type)
		{
			if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE /*|| mbi.Protect == PAGE_EXECUTE_READWRITE*/))
			{				
				if (directAccess)
				{
					const BYTE* pBufferMatch = scanMemory(pattern, mask, pMemChunk, regionSize, alignment);

					if (pBufferMatch != nullptr)
					{
						return reinterpret_cast<UINT64*>(const_cast<BYTE*>(pBufferMatch));
					}
				}
				else
				{
					pBuffer = new BYTE[regionSize];

					DWORD oldprotect;

					if (VirtualProtectEx(hProcess, pMemChunk, regionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
					{
						ReadProcessMemory(hProcess, pMemChunk, pBuffer, regionSize, &bytesRead);
						VirtualProtectEx(hProcess, pMemChunk, regionSize, oldprotect, &oldprotect);

						const BYTE* pBufferMatch = scanMemory(pattern, mask, pBuffer, bytesRead, alignment);

						if (pBufferMatch != nullptr)
						{
							// get memory address of match
							BYTE* pMatch = pMemChunk + (pBufferMatch - pBuffer);

							// clean up buffer
							if (pBuffer)
								delete[] pBuffer;

							pBuffer = nullptr;

							return reinterpret_cast<UINT64*>(pMatch);
						}
					}
				}
			}
		}

		// next chunk
		pMemChunk = pMemChunk + regionSize;

		// clean up buffer
		if (pBuffer)
			delete[] pBuffer;

		pBuffer = nullptr;
	}

	return nullptr;
}


const BYTE* Memory::scanMemory(const char* pattern, const char* mask, const BYTE* pAddress, const SIZE_T bufferSize, ScanAlignment alignment)
{
	SIZE_T patternLength = strlen(mask);

	for (unsigned int pointerOffset = 0; pointerOffset < bufferSize - patternLength; pointerOffset += alignment)
	{
		bool found = true;

		for (unsigned int patternIndex = 0; patternIndex < patternLength; patternIndex++)
		{
			if (mask[patternIndex] != '?' && pattern[patternIndex] != static_cast<char>(*(pAddress + pointerOffset + patternIndex)))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return pAddress + pointerOffset;
		}
	}

	return nullptr;
}



bool Memory::readInt32FromMemory(UINT64* address, int* value, bool readFromLowerDWord, bool directAccess)
{
	bool succeed = false;
	*value = 0;

	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!readFromLowerDWord)
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
			if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY || mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE))
			{
				if (directAccess)
				{
					*value = *reinterpret_cast<int*>(pAddress);
					succeed = true;
				}
				else
				{ 
					BYTE* buffer = new BYTE[READ_SIZE];
					DWORD oldprotect;

					if (VirtualProtectEx(process, pAddress, READ_SIZE, PAGE_EXECUTE_READWRITE, &oldprotect))
					{
						ReadProcessMemory(process, pAddress, buffer, READ_SIZE, NULL);
						VirtualProtectEx(process, pAddress, READ_SIZE, oldprotect, &oldprotect);

						memcpy(value, buffer, READ_SIZE);

						succeed = true;
					}

					delete[] buffer;
				}
			}
		}
	}

	return succeed;
}


bool Memory::readInt64FromMemory(UINT64* address, INT64* value, bool directAccess)
{
	bool succeed = false;
	*value = 0;

	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	MEMORY_BASIC_INFORMATION mbi;

	const SIZE_T READ_SIZE = 8; //8 Bytes

	HANDLE process = GetCurrentProcess();
	if (VirtualQueryEx(process, pAddress, &mbi, sizeof(mbi)))
	{
		if (READ_SIZE <= mbi.RegionSize)
		{
			if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY || mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE))
			{
				if (directAccess)
				{
					*value = *reinterpret_cast<INT64*>(pAddress);
					succeed = true;
				}
				else
				{
					BYTE* buffer = new BYTE[READ_SIZE];
					DWORD oldprotect;

					if (VirtualProtectEx(process, pAddress, READ_SIZE, PAGE_EXECUTE_READWRITE, &oldprotect))
					{
						ReadProcessMemory(process, pAddress, buffer, READ_SIZE, NULL);
						VirtualProtectEx(process, pAddress, READ_SIZE, oldprotect, &oldprotect);

						memcpy(value, buffer, READ_SIZE);

						succeed = true;
					}

					delete[] buffer;
				}
			}
		}
	}

	return succeed;
}


bool Memory::readFloatFromMemory(UINT64* address, float* value, bool readFromLowerDWord, bool directAccess)
{
	*value = 0.0f;
	int intValue = 0;

	if (readInt32FromMemory(address, &intValue, readFromLowerDWord, directAccess))
	{
		*value = *reinterpret_cast<float*>(&intValue);
		return true;
	}

	return false;
}

bool Memory::writeInt32ToMemory(UINT64* address, int value, bool writeToLowerDWord, bool directAccess)
{
	bool succeed = false;

	// convert 64-Bit Pointer 1 Byte Pointer
	BYTE* pAddress = reinterpret_cast<BYTE*>(address);

	if (!writeToLowerDWord)
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
			if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_WRITECOPY || mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_EXECUTE_WRITECOPY))
			{
				if (directAccess)
				{
					*reinterpret_cast<int*>(pAddress) = value;
					succeed = true;
				}
				else
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
	}

	return succeed;
}


bool Memory::writeFloatToMemory(UINT64* address, float value, bool writeToLowerDWord, bool directAccess)
{
	int intValue = *reinterpret_cast<int*>(&value);

	return writeInt32ToMemory(address, intValue, writeToLowerDWord, directAccess);
}
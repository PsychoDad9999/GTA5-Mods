// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <cstddef>

// ----------------------------------------------------------------------------

class AudioMemoryWave
{
public:
	struct chunkHeader
	{
		char	ckID[4];   // Chunk ID: e.g. "data" or "RIFF"
		DWORD   cksize;    // Chunk size		 
	};

public:
	AudioMemoryWave(void* pAddress, std::size_t size, unsigned int volumePercent);
	~AudioMemoryWave();

	bool isValid() { return m_isValid; }

private:	
	chunkHeader* getChunkHeader(BYTE* pAddress, char ckID[4]);
	PCMWAVEFORMAT* getFormatData(chunkHeader* pFormatHeader);

private :
	bool modifyVolume(chunkHeader* pDataHeaderChunk, PCMWAVEFORMAT* pFormat, float volumeMultiplier);
	template <class T> void modifyVolumePCM(BYTE* pAddressDataChunk, DWORD dataSize, float volumeMultiplier, DWORD dwNumSamples);

private:
	bool m_isValid = false;
};

// ----------------------------------------------------------------------------
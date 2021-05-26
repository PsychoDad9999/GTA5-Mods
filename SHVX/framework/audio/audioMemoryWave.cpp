// ----------------------------------------------------------------------------

#include "audioMemoryWave.h"

#include <stdio.h>
#include "framework/system/module.h"


// ----------------------------------------------------------------------------
/// <summery>Constructor.</summery>
/// <param name="pAddress">memory address of wave data. Memory must be writable!</param>
// ----------------------------------------------------------------------------
AudioMemoryWave::AudioMemoryWave(void* pAddress, std::size_t size, int volumePercent)
{
	chunkHeader* pRIFFHeaderChunk = getChunkHeader(reinterpret_cast<BYTE*>(pAddress), "RIFF");

	if (pRIFFHeaderChunk == nullptr)
		return;

	// Validate size
	if (sizeof(chunkHeader) + pRIFFHeaderChunk->cksize != size)	
		return;	

	BYTE* pAddressWaveID = reinterpret_cast<BYTE*>(pRIFFHeaderChunk) + sizeof(chunkHeader);

	if (strncmp(reinterpret_cast<char*>(pAddressWaveID), "WAVE", 4) != 0)	
		return;

	// Get Format Header
	chunkHeader* pFormatHeaderChunk = getChunkHeader(reinterpret_cast<BYTE*>(pAddressWaveID + 4), "fmt ");
	
	if (pFormatHeaderChunk == nullptr)
		return;

	PCMWAVEFORMAT* pFormat = getFormatData(pFormatHeaderChunk);

	if(pFormat == nullptr)			
		return;

	if (pFormat->wf.wFormatTag != WAVE_FORMAT_PCM)
		return;

	chunkHeader* pDataHeaderChunk = getChunkHeader(reinterpret_cast<BYTE*>(pFormat) + pFormatHeaderChunk->cksize, "data");

	// Check if format chunk is valid
	if (pDataHeaderChunk == nullptr)
		return;
	 
	if (volumePercent != 100) // don't modify volume on 100%
	{
		if (volumePercent < 0)
			volumePercent = 0;

		modifyVolume(pDataHeaderChunk, pFormat, volumePercent / 100.0f);
	}
	
	m_isValid = true;
}

// ----------------------------------------------------------------------------
/// <summery>Destructor.</summery>
// ----------------------------------------------------------------------------
AudioMemoryWave::~AudioMemoryWave()
{
	// Nothing to do here
}

// ----------------------------------------------------------------------------

bool AudioMemoryWave::modifyVolume(AudioMemoryWave::chunkHeader* pDataHeaderChunk, PCMWAVEFORMAT* pFormat, float volumeMultiplier)
{
	DWORD dwNumSampleBytes = pDataHeaderChunk->cksize;
	DWORD dwNumSamples = dwNumSampleBytes / (pFormat->wBitsPerSample / 8);
	
	BYTE* pAddressDataChunk = reinterpret_cast<BYTE*>(pDataHeaderChunk) + sizeof(AudioMemoryWave::chunkHeader);

	switch (pFormat->wBitsPerSample)
	{
	case 16:
		modifyVolumePCM<INT16>(pAddressDataChunk, pDataHeaderChunk->cksize, volumeMultiplier, dwNumSamples);		
		return true;

	case 32:
		modifyVolumePCM<INT32>(pAddressDataChunk, pDataHeaderChunk->cksize, volumeMultiplier, dwNumSamples);
		return true;

	default:
		return false;
	}
}

// ----------------------------------------------------------------------------

template <class T> void AudioMemoryWave::modifyVolumePCM(BYTE* pAddressDataChunk, DWORD dataSize, float volumeMultiplier, DWORD dwNumSamples)
{
	DWORD dwBytesPerSample = sizeof(T);
	T* pSample = reinterpret_cast<T*>(pAddressDataChunk);

	for (DWORD dwIndex = 0; dwIndex < dwNumSamples; dwIndex++)
	{
		if (reinterpret_cast<BYTE*>(pSample) + sizeof(T) > pAddressDataChunk + dataSize)
			break;

		T sample = *pSample;
		sample = (T)(sample * volumeMultiplier);
		*pSample = sample;
		pSample++;
	}
}

// ----------------------------------------------------------------------------

AudioMemoryWave::chunkHeader* AudioMemoryWave::getChunkHeader(BYTE* pAddress, char ckID[4])
{
	chunkHeader* pDataHeaderChunk = reinterpret_cast<chunkHeader*>(pAddress);

	// Check if chunk is valid
	if (strncmp(reinterpret_cast<char*>(pDataHeaderChunk->ckID), ckID, 4) != 0)
	{
		return nullptr;
	}

	return pDataHeaderChunk;
}

// ----------------------------------------------------------------------------

PCMWAVEFORMAT* AudioMemoryWave::getFormatData(AudioMemoryWave::chunkHeader* pFormatHeader)
{	
	if (pFormatHeader->cksize == 16 || pFormatHeader->cksize == 18)
	{
		return reinterpret_cast<PCMWAVEFORMAT*>(reinterpret_cast<BYTE*>(pFormatHeader) + sizeof(chunkHeader));
	}	

	return nullptr;
}

// ----------------------------------------------------------------------------
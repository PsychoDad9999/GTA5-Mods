// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <cstddef>

#include "audioMemoryWave.h"

// ----------------------------------------------------------------------------

class AudioMemoryResource
{
public:
	struct Parameters 
	{
		size_t size_bytes = 0;
		void* ptr = nullptr;
	};

public:
	AudioMemoryResource(int resourceId, int volumePercent); // Ctor. Loads a wave audio resource into memory
	~AudioMemoryResource();

	bool playAsync();

	int getVolumePercent() { return m_volumePercent; }

private:
	int m_volumePercent = 100;

private:
	HGLOBAL m_hResourceMemory = nullptr;
	AudioMemoryWave* m_pAudioMemoryWave = nullptr;

	Parameters m_playback;
};

// ----------------------------------------------------------------------------
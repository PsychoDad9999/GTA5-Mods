// ----------------------------------------------------------------------------

#include "audioMemoryResource.h"

#include <stdio.h>
#include "framework/system/module.h"


// ----------------------------------------------------------------------------
/// <summery>Constructor. Loads a wave audio resource into memory</summery>
/// <param name="resourceID">resource ID</param>
// ----------------------------------------------------------------------------
AudioMemoryResource::AudioMemoryResource(int resourceId, int volumePercent)
{
	m_playback.size_bytes = 0;
	m_playback.ptr = nullptr;

	HMODULE hInst = Module::getModule();

	HRSRC hResource = FindResource(hInst, MAKEINTRESOURCE(resourceId), TEXT("WAVE"));

	if (hResource == nullptr)
		return;
		
	// Load the WAVE resource. 
	m_hResourceMemory = LoadResource(hInst, hResource);

	if (m_hResourceMemory != nullptr)
	{
		size_t resourceSize = SizeofResource(hInst, hResource);
		void* pMemResource = LockResource(m_hResourceMemory);

		if (pMemResource == nullptr)
		{
			// failure
			FreeResource(m_hResourceMemory);
			m_hResourceMemory = nullptr;
		}
		else
		{
			printf("Audio Resource (%i): %p, Size: %zi\n", resourceId, pMemResource, resourceSize);

			bool changeVolumeLevel = true;

			if (changeVolumeLevel)
			{
				// make a copy of the resource memory, because it's read-only
				BYTE* pAudioBuffer = new BYTE[resourceSize];
				memcpy(pAudioBuffer, pMemResource, resourceSize);

				m_pAudioMemoryWave = new AudioMemoryWave(pAudioBuffer, resourceSize, volumePercent);

				if (m_pAudioMemoryWave->isValid())
				{
					// free resource, use AudioMemoryWave, instead of resource mem
					FreeResource(m_hResourceMemory);
					m_hResourceMemory = nullptr;

					// change playback pointer
					m_playback.ptr = pAudioBuffer;
					m_playback.size_bytes = resourceSize;

					printf("Using AudioMemoryWave\n");
				}
				else
				{
					delete[] pAudioBuffer;
					pAudioBuffer = nullptr;

					delete m_pAudioMemoryWave;
					m_pAudioMemoryWave = nullptr;

					// failed to create AudioMemoryWave, use Memresource as fallback
					m_playback.ptr = pMemResource;
					m_playback.size_bytes = resourceSize;

					printf("Failed to create AudioMemoryWave. Using Memresource.\n");
				}
			}
			else
			{
				// Audio level not changing. No need to create an AudioMemoryWave object
				m_playback.ptr = pMemResource;
				m_playback.size_bytes = resourceSize;
			}
		}		
	}
}

// ----------------------------------------------------------------------------
/// <summery>Destructor. Frees the audio memory resource.</summery>
/// <remarks>Stops playback of any sound that is playing asynchronously</remarks>
// ----------------------------------------------------------------------------
AudioMemoryResource::~AudioMemoryResource()
{
	// the memory buffer that contains the sound must remain valid until the sound has completed playing.
	// stop playback of an sound that is playing asynchronously
	sndPlaySound(nullptr, 0);

	if (m_hResourceMemory != nullptr)
	{
		// failure
		FreeResource(m_hResourceMemory);
		m_hResourceMemory = nullptr;
		m_playback.ptr = nullptr;
	}

	if (m_pAudioMemoryWave != nullptr)
	{
		delete[] m_playback.ptr;
		m_playback.ptr = nullptr;

		delete m_pAudioMemoryWave;
		m_pAudioMemoryWave = nullptr;
	}
}


// ----------------------------------------------------------------------------
/// <summery>Play audio memory resource async</summery>
/// <returns>Retruns true if audio resource is valid, otherwise false</returns>
// ----------------------------------------------------------------------------
bool AudioMemoryResource::playAsync()
{
	if (m_playback.ptr != nullptr)
	{
		sndPlaySound(reinterpret_cast<LPCWSTR>(m_playback.ptr), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		return true;
	}
	else
	{
		return false;
	}
}

// ----------------------------------------------------------------------------
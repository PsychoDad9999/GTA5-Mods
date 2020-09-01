// ----------------------------------------------------------------------------

#include "audioMemoryResource.h"

#include <stdio.h>

#include "framework/system/module.h"

// ----------------------------------------------------------------------------
/// <summery>Constructor. Loads a wave audio resource into memory</summery>
/// <param name="resourceID">resource ID</param>
// ----------------------------------------------------------------------------
AudioMemoryResource::AudioMemoryResource(int resourceId)
{
	HMODULE hInst = Module::getModule();

	m_hResource = FindResource(hInst, MAKEINTRESOURCE(resourceId), TEXT("WAVE"));

	if (m_hResource == nullptr)
	{
		return;
	}
		
	// Load the WAVE resource. 
	m_hMemory = LoadResource(hInst, m_hResource);

	if (m_hMemory != nullptr)
	{
		m_parameter.size_bytes = SizeofResource(nullptr, m_hResource);
		m_parameter.ptr = LockResource(m_hMemory);

		if (m_parameter.ptr == nullptr)
		{
			// failure
			FreeResource(m_hMemory);
			m_hMemory = nullptr;
			m_hResource = nullptr;
		}
	}
	else
	{
		m_hResource = nullptr;
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

	if (m_hMemory != nullptr)
	{
		// failure
		FreeResource(m_hMemory);
		m_hMemory = nullptr;
		m_hResource = nullptr;
		m_parameter.ptr = nullptr;
	}
}


// ----------------------------------------------------------------------------
/// <summery>Play audio memory resource async</summery>
/// <returns>Retruns true if audio resource is valid, otherwise false</returns>
// ----------------------------------------------------------------------------
bool AudioMemoryResource::playAsync()
{
	if (m_parameter.ptr != nullptr)
	{
		sndPlaySound(reinterpret_cast<LPCWSTR>(m_parameter.ptr), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		return true;
	}
	else
	{
		return false;
	}
}

// ----------------------------------------------------------------------------
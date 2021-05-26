// ----------------------------------------------------------------------------

#include "audioPlayer.h"

// ----------------------------------------------------------------------------

std::map<int, AudioMemoryResource*> AudioPlayer::m_soundDictionary;

// ----------------------------------------------------------------------------
/// <summery>Play a sound from a resource</summery>
/// <param name="resourceID">resource id</param>
// ----------------------------------------------------------------------------
void AudioPlayer::play(int resourceID, unsigned int volumePercent)
{	
	if (!contains(resourceID))
	{
		// Create sound dynamically and add to dictionary
		add(resourceID, volumePercent);
	}

	// Get Audio Memory Resource from Dictionary
	AudioMemoryResource* pAudio = get(resourceID);

	if (pAudio != nullptr)
	{
		pAudio->playAsync();
	}
}


// ----------------------------------------------------------------------------
/// <summery>Check if sound is available as audio memory resource</summery>
/// <param name="resourceID">resource id</param>
/// <returns>true if audio memory resource is available, otherwise false</returns>
// ----------------------------------------------------------------------------
bool AudioPlayer::contains(const int resourceID)
{
	return m_soundDictionary.find(resourceID) != m_soundDictionary.end();
}


// ----------------------------------------------------------------------------
/// <summery>Get audio memory resource</summery>
/// <param name="sound">resource id</param>
/// <returns>Pointer of audio memory resource or nullptr</returns>
// ----------------------------------------------------------------------------
AudioMemoryResource* AudioPlayer::get(const int resourceID)
{
	if (contains(resourceID))
	{
		return m_soundDictionary[resourceID];
	}
	else
	{
		return nullptr;
	}
}


// ----------------------------------------------------------------------------
/// <summery>Add sound to audioplayer dictionary</summery>
/// <param name="resourceID">resource id</param>
/// <returns>True if new audio memory resource has been created and added to the dictionary, otherwise false</returns>
// ----------------------------------------------------------------------------
bool AudioPlayer::add(const int resourceID, int volumePercent)
{
	if (contains(resourceID))
		return false;

	AudioMemoryResource* pSound = new AudioMemoryResource(resourceID, volumePercent);

	m_soundDictionary[resourceID] = pSound;
	
	return true;
}


// ----------------------------------------------------------------------------
/// <summery>Dispose audioplayer and cleanup all audio memory resources</summery>
// ----------------------------------------------------------------------------
void AudioPlayer::dispose()
{
	std::map<int, AudioMemoryResource*>::iterator it;

	for (it = m_soundDictionary.begin(); it != m_soundDictionary.end(); it++)
	{
		AudioMemoryResource* pSound = it->second;

		if (pSound != nullptr)
		{
			delete pSound;
			pSound = nullptr;
		}
	}	
}


// ----------------------------------------------------------------------------
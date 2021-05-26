// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <map>

#include "audioMemoryResource.h"

// ----------------------------------------------------------------------------

class AudioPlayer
{
public:
	static void dispose();
	static void play(int resourceID, unsigned int volumePercent);

private:
	static bool contains(const int resourceID);
	static bool add(const int resourceID, int volumePercent);
	static AudioMemoryResource* get(const int resourceID);
	
private:
	static std::map<int, AudioMemoryResource*> m_soundDictionary;	
};

// ----------------------------------------------------------------------------

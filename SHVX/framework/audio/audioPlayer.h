// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <map>

#include "framework/audio/audioMemoryResource.h"

// ----------------------------------------------------------------------------

class AudioPlayer
{
public:
	static void dispose();
	static void play(int resourceID);	

private:
	static bool contains(const int resourceID);
	static bool add(const int resourceID);
	static AudioMemoryResource* get(const int resourceID);
	
private:
	static std::map<int, AudioMemoryResource*> m_soundDictionary;	
};

// ----------------------------------------------------------------------------

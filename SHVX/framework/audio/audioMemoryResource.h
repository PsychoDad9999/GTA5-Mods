// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <cstddef>

// ----------------------------------------------------------------------------

class AudioMemoryResource
{
public:
	struct Parameters 
	{
		std::size_t size_bytes = 0;
		void* ptr = nullptr;
	};

public:
	AudioMemoryResource(int resourceId); // Ctor. Loads a wave audio resource into memory
	~AudioMemoryResource();

	bool playAsync();

private:
	HRSRC m_hResource = nullptr;
	HGLOBAL m_hMemory = nullptr;

	Parameters m_parameter;
};

// ----------------------------------------------------------------------------
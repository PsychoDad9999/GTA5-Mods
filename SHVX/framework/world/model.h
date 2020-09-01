// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"

// ----------------------------------------------------------------------------

class Model
{
public:
	// Request Model from Streaming Engine
	static bool requestModel(Hash modelHash, int timeout);
	
	// Unloads Model from memory
	static bool unloadModel(Hash modelHash);

private:
	Model() {};
};

// ----------------------------------------------------------------------------
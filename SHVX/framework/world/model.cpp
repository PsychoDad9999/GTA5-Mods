// ----------------------------------------------------------------------------

#include "model.h"

#include <chrono>

#include "inc/natives.h"

// ----------------------------------------------------------------------------

bool Model::requestModel(Hash modelHash, int timeout)
{
	auto now = std::chrono::high_resolution_clock::now();

	STREAMING::REQUEST_MODEL(modelHash);

	while (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		WAIT(0);
		STREAMING::REQUEST_MODEL(modelHash);

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() >= timeout)
		{
			// Timeout
			return STREAMING::HAS_MODEL_LOADED(modelHash);
		}
	}

	return true;
}

// ----------------------------------------------------------------------------

bool Model::unloadModel(Hash modelHash)
{
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
}

// ----------------------------------------------------------------------------
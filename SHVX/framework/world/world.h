// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"

// ----------------------------------------------------------------------------

class World
{
public:
	// Get Vector3D of specified entity
	static Vector3 getPosition(Entity entity);
	
	static double getDistance(Vector3 pos1, Vector3 pos2, bool ignoreZ);

	static float getGroundHeight(Vector3 position);

private:
	World() {};
};

// ----------------------------------------------------------------------------
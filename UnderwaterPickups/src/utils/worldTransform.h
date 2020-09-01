// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"

// ----------------------------------------------------------------------------

class Transform
{
public:
	// Get Vector3D of specified entity
	static Vector3 getWorldPosition(Entity entity);

	// Get horizontal distance. Ignores Z Axis of Vecor Points
	static double getHorizontalDistance(Vector3 pos1, Vector3 pos2);

	static double getDistance(Vector3 pos1, Vector3 pos2);

private:
	Transform() {};
};

// ----------------------------------------------------------------------------
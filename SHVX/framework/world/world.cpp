// ----------------------------------------------------------------------------

#include "world.h"

#include "inc/natives.h"

#include <math.h>

// ----------------------------------------------------------------------------

Vector3 World::getPosition(Entity entity)
{
	return ENTITY::GET_ENTITY_COORDS(entity, TRUE);
}


double World::getDistance(Vector3 pos1, Vector3 pos2, bool ignoreZ)
{
	if (ignoreZ)
	{
		return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
	}
	else
	{
		return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2));
	}
}



float World::getGroundHeight(Vector3 position)
{
	float groundZ = 0;

	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(position.x, position.y, position.z, &groundZ, FALSE);

	return groundZ;
}

// ----------------------------------------------------------------------------
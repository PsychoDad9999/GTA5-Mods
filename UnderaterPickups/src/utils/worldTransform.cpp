// ----------------------------------------------------------------------------

#include "worldTransform.h"

#include <math.h>

#include "inc/natives.h"

// ----------------------------------------------------------------------------

Vector3 Transform::getWorldPosition(Entity entity)
{
	return ENTITY::GET_ENTITY_COORDS(entity, TRUE);
}


double Transform::getHorizontalDistance(Vector3 pos1, Vector3 pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

double Transform::getDistance(Vector3 pos1, Vector3 pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2));
}


// ----------------------------------------------------------------------------
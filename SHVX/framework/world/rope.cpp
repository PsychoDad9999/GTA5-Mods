// ----------------------------------------------------------------------------

#include "rope.h"

#include <chrono>

#include "../SHV/inc/natives.h"
#include "framework/world/world.h"

// ----------------------------------------------------------------------------

Object Rope::createRope(eRopeType ropeType, Vector3 position, float length, float minLength, bool breakable)
{
	// load rope textures
	ROPE::ROPE_LOAD_TEXTURES();

	// create rope
	return ROPE::ADD_ROPE(position.x, position.y, position.z, 0, 0, 0, length, ropeType, length, minLength, 0.5f, FALSE, FALSE, TRUE, 1.0f, breakable, NULL);
}

// ----------------------------------------------------------------------------

void Rope::deleteRope(Object& ropeHandle)
{
	if (ropeHandle != 0)
	{
		if (ROPE::DOES_ROPE_EXIST(&ropeHandle))
		{
			ROPE::DELETE_ROPE(&ropeHandle);
		}

		ropeHandle = 0;
	}	
}

// ----------------------------------------------------------------------------

void Rope::attachEntities(Object ropeHandle, Entity entity1, Entity entity2, float ropeLength)
{	
	attachEntities(ropeHandle, entity1, World::getPosition(entity1), entity2, World::getPosition(entity2), ropeLength);	
}

// ----------------------------------------------------------------------------

void Rope::attachEntities(Object ropeHandle, Entity entity1, Vector3 pos1, Entity entity2, Vector3 pos2, float ropeLength)
{
	ROPE::ATTACH_ENTITIES_TO_ROPE(ropeHandle, entity1, entity2, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, ropeLength, FALSE, FALSE, NULL, NULL);
}

// ----------------------------------------------------------------------------

void Rope::enablePhysics(Object ropeHandle)
{
	ROPE::ACTIVATE_PHYSICS(ropeHandle);
}

// ----------------------------------------------------------------------------
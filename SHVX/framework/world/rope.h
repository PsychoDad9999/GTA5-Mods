// ----------------------------------------------------------------------------

#pragma once

#include "../SHV/inc/types.h"

// ----------------------------------------------------------------------------

enum eRopeType : int
{
	THIN_ROPE = 1,
	THICK_ROPE = 2,
	THICK_CABLE = 3,
	MEDIUM_CABLE = 4,
	THIN_CABLE = 5,
};


class Rope
{
public:
	static Object createRope(eRopeType ropeType, Vector3 position, float length, float minLength, bool breakable);

	static void deleteRope(Object& ropeHandle);

	static void attachEntities(Object ropeHandle, Entity entity1, Entity entity2, float ropeLength);
	static void attachEntities(Object ropeHandle, Entity entity1, Vector3 pos1, Entity entity2, Vector3 pos2, float ropeLength);

	static void enablePhysics(Object ropeHandle);

private:
	Rope() {};
};

// ----------------------------------------------------------------------------
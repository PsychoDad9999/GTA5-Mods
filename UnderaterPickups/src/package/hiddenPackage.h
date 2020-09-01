// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"
#include "framework/system/dateTime.h"

#include "time/timeHelper.h"

// ----------------------------------------------------------------------------

struct HiddenPackage 
{
	int id;
	Vector3 scriptActivationLocation;
	Vector3 packageLocation;
	DateTime nextSpawnTime;
	bool isPickupInRange;

	HiddenPackage() : HiddenPackage(-1, Vector3(), Vector3()) {}

	HiddenPackage(int uniqueId, Vector3 scriptActivationLoc, Vector3 packageLoc) : id(uniqueId), scriptActivationLocation(scriptActivationLoc), packageLocation(packageLoc), nextSpawnTime(TimeHelper::TIME_NOT_SET), isPickupInRange(false){}
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"
#include "vehicle/vehicleEntity.h"

// ----------------------------------------------------------------------------

class DebugVehicleSpawner
{
public:
	static void spawnVehicleAtPedLocation(Ped ped, VehicleEntity::eVehicleHash vehicleHash);

private:
	DebugVehicleSpawner() {}
};

// ----------------------------------------------------------------------------
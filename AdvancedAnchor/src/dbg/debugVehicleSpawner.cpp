// ----------------------------------------------------------------------------

#include "debugVehicleSpawner.h"

#include "inc/natives.h"
#include "framework/world/world.h"

// ----------------------------------------------------------------------------
/// <summery>Spawn a vehicle close to ped position</summery>
/// <param name="ped">ped</param>
/// <param name="vehicleHash">hash of vehicle that will be spawned</param>
// ----------------------------------------------------------------------------
void DebugVehicleSpawner::spawnVehicleAtPedLocation(Ped ped, VehicleEntity::eVehicleHash vehicleHash)
{
	if (vehicleHash == VehicleEntity::eVehicleHash::VehicleHashInvalid)
		return;

	Vector3 pedPos = World::getPosition(ped);

	// offset position
	pedPos.x += 10;
	pedPos.y += 10;

	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(vehicleHash, pedPos.x, pedPos.y, pedPos.z, 0, FALSE, FALSE);

	OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(vehicle);
}

// ----------------------------------------------------------------------------

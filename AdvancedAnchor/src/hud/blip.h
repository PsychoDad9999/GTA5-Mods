// ----------------------------------------------------------------------------

#pragma once

#include "inc\types.h"
#include "vehicle\anchoredVehicle.h"

// ----------------------------------------------------------------------------

class MapBlip
{
public:
	static bool add(Vehicle vehicle, eVehicleType vehicleType, bool showOnMap, bool isShortRange);

	static void remove(Vehicle vehicle);

private:
	MapBlip() {}

};

// ----------------------------------------------------------------------------
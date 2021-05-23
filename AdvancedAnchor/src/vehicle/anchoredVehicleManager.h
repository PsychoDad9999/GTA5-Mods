// ----------------------------------------------------------------------------

#pragma once

#include <map>

#include "anchoredVehicle.h"

// ----------------------------------------------------------------------------

class AnchoredVehicleManager
{
public:
	bool contains(const Vehicle& vehicle);

	bool add(const Vehicle& vehicle, eVehicleType vehicleType);

	bool remove(const Vehicle& vehicle);

	void clear();

	AnchoredVehicle* const get(const Vehicle& vehicle);

	AnchoredVehicle* const findFirstSecuredVehicle(Vector3 fromPosition, double minDistance);
	AnchoredVehicle* const findNearestSecuredVehicle(Vector3 fromPosition, double maxDistance, eVehicleType vehicleType);

	// for logging only
	AnchoredVehicle* const getLast();

private:
	std::map<Vehicle, AnchoredVehicle> m_anchoredVehicleDictionary;
	
	// for logging only
	AnchoredVehicle* m_lastAnchoredVehicle = nullptr;
};

// ----------------------------------------------------------------------------
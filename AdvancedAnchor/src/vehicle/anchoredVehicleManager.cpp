// ----------------------------------------------------------------------------

#include "anchoredVehicleManager.h"

#include "framework/world/world.h"

// ----------------------------------------------------------------------------

bool AnchoredVehicleManager::contains(const Vehicle& vehicle)
{
	return m_anchoredVehicleDictionary.find(vehicle) != m_anchoredVehicleDictionary.end();
}


bool AnchoredVehicleManager::add(const Vehicle& vehicle, eVehicleType vehicleType)
{
	if (contains(vehicle))
		return false;

	AnchoredVehicle anchoredVehicle;
	anchoredVehicle.vehicle = vehicle;
	anchoredVehicle.vehicleType = vehicleType;

	m_anchoredVehicleDictionary[vehicle] = anchoredVehicle;

	return true;
}


bool AnchoredVehicleManager::remove(const Vehicle& vehicle)
{
	if (!contains(vehicle))
		return false;

	m_anchoredVehicleDictionary.erase(vehicle);
	m_lastAnchoredVehicle = nullptr;

	return true;
}


void AnchoredVehicleManager::clear()
{
	m_anchoredVehicleDictionary.clear();
	m_lastAnchoredVehicle = nullptr;
}



AnchoredVehicle* const AnchoredVehicleManager::get(const Vehicle& vehicle)
{
	if (contains(vehicle))
	{
		m_lastAnchoredVehicle = &m_anchoredVehicleDictionary[vehicle];

		return m_lastAnchoredVehicle;
	}
	else
	{
		return nullptr;
	}
}


AnchoredVehicle* const AnchoredVehicleManager::findFirstSecuredVehicle(Vector3 fromPosition, double minDistance)
{
	std::map<Vehicle, AnchoredVehicle>::iterator it;

	for (it = m_anchoredVehicleDictionary.begin(); it != m_anchoredVehicleDictionary.end(); it++)
	{
		AnchoredVehicle* const anchoredVehicle = &it->second;

		if (anchoredVehicle->isSecured)
		{			
			double blipDistance = World::getDistance(fromPosition, anchoredVehicle->blipPosition, true);

			if (blipDistance > minDistance)
				return anchoredVehicle;
		}
	}
		
	return nullptr;
}



AnchoredVehicle* const AnchoredVehicleManager::findNearestSecuredVehicle(Vector3 fromPosition, double maxDistance, eVehicleType vehicleType)
{
	AnchoredVehicle* match = nullptr;
	double matchDistance = 0;

	std::map<Vehicle, AnchoredVehicle>::iterator it;

	for (it = m_anchoredVehicleDictionary.begin(); it != m_anchoredVehicleDictionary.end(); it++)
	{
		AnchoredVehicle* const anchoredVehicle = &it->second;

		if (anchoredVehicle->isSecured && anchoredVehicle->vehicleType == vehicleType)
		{
			double blipDistance = World::getDistance(fromPosition, anchoredVehicle->blipPosition, false);
		
			if (blipDistance < maxDistance)
			{
				if (match == nullptr || blipDistance < matchDistance)
				{
					match = anchoredVehicle;
					matchDistance = blipDistance;
				}				
			}
		}
	}

	return match;
}





AnchoredVehicle* const AnchoredVehicleManager::getLast()
{
	return m_lastAnchoredVehicle;
}

// ----------------------------------------------------------------------------
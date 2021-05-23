// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

#include "inc/types.h"

// ----------------------------------------------------------------------------

enum eVehicleType
{
	VehicleTypeUnknown = 0,
	VehicleTypeBoat = 1,
	VehicleTypeJetSki = 2,
	VehicleTypeSub = 3,
	VehicleTypePlane = 4,
	VehicleTypeHeli = 5,
};



struct AnchoredVehicle
{
	struct BreakingData
	{
		bool isInitialized;
		ULONGLONG breakingStartTime;
		double initialSpeed;
		ULONGLONG totalTimeToStop;
		double breakingDistanceInMeters;
		bool hasStopped;
		
		BreakingData() : isInitialized(false), breakingStartTime(0), initialSpeed(0), totalTimeToStop(0), breakingDistanceInMeters(0), hasStopped(false)
		{}
	};

	struct RopeData
	{
		Object ropeHandle;
		Entity anchorEntity;

		RopeData() : ropeHandle(0), anchorEntity(0)
		{}
	};

	Vehicle vehicle;  // Vehicle ID
	bool isAnchored;  // True if vehicle is anchored or in the process of being anchored
	bool isSecured;   // True if player has left the vehicle and vehicle coordinates has been secured by the game. Game-Engine will not remove a secured vehicle
	bool isReentered; // True if player has re-entered a secured vehicle.
	Vector3 blipPosition; // Blip Position
	eVehicleType vehicleType; //Vehicle Type
	BreakingData breakingData; // Only used by Sea-Plane
	RopeData ropeData; // Only used by submersible

	AnchoredVehicle() : vehicle(NULL), isAnchored(false), isSecured(false), isReentered(false), blipPosition(), vehicleType(eVehicleType::VehicleTypeUnknown), breakingData(), ropeData()
	{
	}
};

// ----------------------------------------------------------------------------

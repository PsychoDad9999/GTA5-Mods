// ----------------------------------------------------------------------------

#pragma once

#include <math.h>

#include "inc/natives.h"
#include "anchoredVehicle.h"
#include "framework/world/physics.h"


// ----------------------------------------------------------------------------

class VehicleEntity
{
public:
	enum eVehicleHash
	{
		VehicleHashInvalid = 0x0,
		VehicleHashDinghy1 = 0x3D961290,
		VehicleHashDinghy2 = 0x107F392C,  // 2 seater
		VehicleHashSeashark1 = 0xC2974024, // Speedophile Seashark
		VehicleHashSeashark2 = 0xDB4388E4, // Lifeguard Seashark
		VehicleHashSubmersible1 = 0x2DFF622F,
		VehicleHashSubmersible2 = 0xC07107EE, // Kraken
		VehicleHashYacht1 = 0xC1CE1183,
	};


public:
	static eVehicleType getVehicleType(Vehicle vehicle, bool isPlayerInAnySub)
	{
		const Hash modelHash = ENTITY::GET_ENTITY_MODEL(vehicle);

		if (VEHICLE::IS_THIS_MODEL_A_PLANE(modelHash))
			return eVehicleType::VehicleTypePlane;
		else if (VEHICLE::_IS_THIS_MODEL_A_JETSKI(modelHash))
			return eVehicleType::VehicleTypeJetSki;
		else if (VEHICLE::IS_THIS_MODEL_A_BOAT(modelHash))	// check Jetski first because IS_THIS_MODEL_A_BOAT will return true on jetski's too
			return eVehicleType::VehicleTypeBoat;
		else if(VEHICLE::IS_THIS_MODEL_A_HELI(modelHash))
			return eVehicleType::VehicleTypeHeli;
		else if (isPlayerInAnySub)	// there is no IS_MODEL Function to check if the vehicle is an submersible
			return eVehicleType::VehicleTypeSub;
		else
			return eVehicleType::VehicleTypeUnknown;
	}


	static double getVehicleSpeedKmph(Vehicle vehicle)
	{
		return Physics::mpsToKmph(getVehicleSpeedmps(vehicle));
	}

	static double getVehicleSpeedmps(Vehicle vehicle)
	{
		Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(vehicle);

		// speed in m/s
		return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));		
	}


	static double getVehicleForwardSpeedmps(Vehicle vehicle)
	{
		Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(vehicle);

		// speed in m/s
		return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
	}


	static bool isDrivingBackwards(Vehicle vehicle)
	{
		Vector3 speedVector = ENTITY::GET_ENTITY_SPEED_VECTOR(vehicle, true);
		
		return speedVector.y < 0;
	}


private:
	VehicleEntity() {}

};
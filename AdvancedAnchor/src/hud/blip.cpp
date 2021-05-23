// ----------------------------------------------------------------------------

#include "blip.h"

#include "inc/natives.h"
#include "inc/enums.h"


// ----------------------------------------------------------------------------
/// <summery>Additional Blip Sprites that are missing in eBlipSprite</summery>
// ----------------------------------------------------------------------------
enum eAdditionalBlipSprite
{
	BlipSpriteYacht = 455,
	BlipSpriteJetSki = 471,
	BlipSpritePlaneMogul = 578,
	BlipSpritePlaneV65Molotok = 579,
	BlipSpritePlaneP45Nokota = 580,
	BlipSpritePlanePyro = 581,
	BlipSpritePlaneRogue = 582,
	BlipSpriteHeliSeaSparrow = 612,
};


// ----------------------------------------------------------------------------
/// <summery>Add a vehicle map blip and make vehicle persistent</summery>
/// <param name="vehicle">vehicle</param>
/// <param name="vehicleType">vehicle type</param>
/// <param name="showOnMap">If false the vehicle is made persistent, but no map blip will be created</param>
/// <param name="isShortRange">Use short-range blip or long-range blip</param>
/// <returns>Retruns false if blip already exist, otherwise true</returns>
// ----------------------------------------------------------------------------
bool MapBlip::add(Vehicle vehicle, eVehicleType vehicleType, bool showOnMap, bool isShortRange) 
{
	Blip blip = UI::GET_BLIP_FROM_ENTITY(vehicle);

	if (UI::DOES_BLIP_EXIST(blip))
	{
		// Blip already exist
		return false;
	}
	else
	{
		if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(vehicle))
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, TRUE, FALSE);
		}

		if (showOnMap)
		{
			Blip blip = UI::ADD_BLIP_FOR_ENTITY(vehicle);
			UI::SET_BLIP_SCALE(blip, 1.0);

			if (isShortRange)
			{
				UI::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
			}

			switch (vehicleType)
			{
			case eVehicleType::VehicleTypeBoat:
				UI::SET_BLIP_SPRITE(blip, eBlipSprite::BlipSpriteDinghy);
				break;

			case eVehicleType::VehicleTypeJetSki:
				UI::SET_BLIP_SPRITE(blip, eAdditionalBlipSprite::BlipSpriteJetSki);
				break;

			case eVehicleType::VehicleTypeSub:
				UI::SET_BLIP_SPRITE(blip, eBlipSprite::BlipSpriteSub);
				break;

			case eVehicleType::VehicleTypePlane:
				UI::SET_BLIP_SPRITE(blip, eBlipSprite::BlipSpritePlane);
				break;

			case eVehicleType::VehicleTypeHeli:
				UI::SET_BLIP_SPRITE(blip, eAdditionalBlipSprite::BlipSpriteHeliSeaSparrow);
				break;

			default:
			case eVehicleType::VehicleTypeUnknown:
				UI::SET_BLIP_COLOUR(blip, 0xFF77FF);
				break;
			}
		}
	}

	return true;
}


// ----------------------------------------------------------------------------
/// <summery>Remove a vehicle map blip and mark vehicle as no longer needed</summery>
/// <param name="vehicle">vehicle</param>
// ----------------------------------------------------------------------------
void MapBlip::remove(Vehicle vehicle)
{
	Blip blip = UI::GET_BLIP_FROM_ENTITY(vehicle);
	
	if (UI::DOES_BLIP_EXIST(blip))
	{
		//Remove Blip
		UI::REMOVE_BLIP(&blip);
	}

	ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);
}

// ----------------------------------------------------------------------------
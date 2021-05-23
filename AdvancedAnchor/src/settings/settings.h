// ----------------------------------------------------------------------------

#pragma once

#include <windows.h>

#include "vehicle/anchoredVehicle.h"

// ----------------------------------------------------------------------------

class Settings
{
public:
	struct ModSettings
	{
		// Anchor Behavior Settings
		struct BehaviorSettings
		{
			bool autoRetrieveAnchorWhenGetInVehicle;
			bool autoDeployAnchorWhenLeaveVehicle;
			unsigned int anchorAutoRemovalDistance;  // 0 = No removal distance

			BehaviorSettings() : autoRetrieveAnchorWhenGetInVehicle(true), autoDeployAnchorWhenLeaveVehicle(true), anchorAutoRemovalDistance(2000) {}
		};

		// Allowed Vehicles
		struct VehicleSettings
		{
			bool allowBoat;
			bool allowJetski;
			bool allowSubmersible;
			bool allowSeaPlanes;
			bool allowSeaHelicopters;

			VehicleSettings() : allowBoat(true), allowJetski(true), allowSubmersible(true), allowSeaPlanes(true), allowSeaHelicopters(true) {}
		};


		struct AudioSettings
		{
			bool allowAnchorSounds;

			AudioSettings() : allowAnchorSounds(true) {}
		};


		struct HUDSettings
		{
			bool allowNotifications;
			ULONG notificationDuration;
			bool allowHelpMessages;
			bool showAnchoredVehiclesOnMap;
			bool useShortRangeMapMarkers;

			HUDSettings() : allowNotifications(true), notificationDuration(3000), allowHelpMessages(true), showAnchoredVehiclesOnMap(true), useShortRangeMapMarkers(true) {}
		};

		struct KeySettings
		{
			DWORD anchorKey;			

			KeySettings() : anchorKey(0) {}
		};


		struct MiscSettings
		{
			bool allowHoldKeyToEnterSubmersible;

			MiscSettings() : allowHoldKeyToEnterSubmersible(true) {}
		};


		BehaviorSettings behavior;
		VehicleSettings vehicles;
		AudioSettings audio;		
		HUDSettings hud;
		KeySettings input;
		MiscSettings misc;

		ModSettings() : behavior(), vehicles(), audio(), hud(), input(), misc() {}

		bool isVehicleTypeAllow(const eVehicleType vehicleType)
		{
			switch (vehicleType)
			{
			case eVehicleType::VehicleTypeJetSki:
				return vehicles.allowJetski;

			case eVehicleType::VehicleTypeBoat:
				return vehicles.allowBoat;

			case eVehicleType::VehicleTypeSub:
				return vehicles.allowSubmersible;

			case eVehicleType::VehicleTypePlane:
				return vehicles.allowSeaPlanes;

			case eVehicleType::VehicleTypeHeli:
				return vehicles.allowSeaHelicopters;

			default:
			case eVehicleType::VehicleTypeUnknown:
				return false;
			}
		}

	};

public:
	static bool loadFromFile(ModSettings& settings, const wchar_t* filename);

private:
	Settings() {};

};

// ----------------------------------------------------------------------------
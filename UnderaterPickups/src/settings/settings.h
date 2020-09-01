// ----------------------------------------------------------------------------

#pragma once

#include <windows.h>

// ----------------------------------------------------------------------------

class Settings
{
public:
	struct ModSettings
	{

		struct BehaviorSettings
		{
			unsigned int respawnTimeInHours;			

			BehaviorSettings() : respawnTimeInHours(24) {}
		};

		struct HUDSettings
		{
			bool showRespawnNotification;
			bool showUnsupportedVersionNotification;
			ULONG notificationDuration;
			bool showPackageBlips;

			HUDSettings() : showRespawnNotification(true), showUnsupportedVersionNotification(true), notificationDuration(3000), showPackageBlips(false){}
		};


		BehaviorSettings behavior;
		HUDSettings hud;

		ModSettings() : behavior(), hud() {}
	};

public:
	static bool loadFromFile(ModSettings& settings, const wchar_t* filename);

private:
	Settings() {};

};

// ----------------------------------------------------------------------------
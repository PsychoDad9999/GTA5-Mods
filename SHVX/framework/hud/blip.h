// ----------------------------------------------------------------------------

#pragma once

#include "../SHV/inc/types.h"

// ----------------------------------------------------------------------------

class MapBlip
{
public:
	static Blip AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID);
	static Blip AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, float scale);
	static Blip AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, const char* name);
	static Blip AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, float scale, const char* name);

	static Blip AddBlipForEntity(Entity entity, int spriteID, const char* name);
	static Blip AddBlipForEntity(Entity entity, int spriteID, const char* name, bool showRoute);
	static Blip AddBlipForEntity(Entity entity, int spriteID, const char* name, bool showRoute, int color);

	static void RemoveBlip(Blip& blip);

private:
	MapBlip();

};

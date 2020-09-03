// ----------------------------------------------------------------------------

#include "blip.h"

#include <inc/natives.h>

// ----------------------------------------------------------------------------

Blip MapBlip::AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID)
{
	return AddBlipForCoordinate(location, isShortRange, spriteID, 1.f, nullptr);
}


Blip MapBlip::AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, float scale)
{
	return AddBlipForCoordinate(location, isShortRange, spriteID, scale, nullptr);
}

Blip MapBlip::AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, const char* name)
{
	return AddBlipForCoordinate(location, isShortRange, spriteID, 1.f, name);
}

Blip MapBlip::AddBlipForCoordinate(Vector3 location, bool isShortRange, int spriteID, float scale, const char* name)
{
	Blip blip = UI::ADD_BLIP_FOR_COORD(location.x, location.y, location.z);

	if (isShortRange)
	{
		UI::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
	}

	if (spriteID > 0 && spriteID < 670)
	{
		UI::SET_BLIP_SPRITE(blip, spriteID);
	}

	UI::SET_BLIP_SCALE(blip, scale);

	if (name != nullptr)
	{
		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(name));
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
	}

	return blip;
}

Blip MapBlip::AddBlipForEntity(Entity entity, int spriteID, const char* name)
{
	return AddBlipForEntity(entity, spriteID, name, false, -1);
}

Blip MapBlip::AddBlipForEntity(Entity entity, int spriteID, const char* name, bool showRoute)
{
	return AddBlipForEntity(entity, spriteID, name, showRoute, -1);
}

Blip MapBlip::AddBlipForEntity(Entity entity, int spriteID, const char* name, bool showRoute, int color)
{
	Blip blip = UI::ADD_BLIP_FOR_ENTITY(entity);

	if (spriteID > 0 && spriteID < 670)
	{
		UI::SET_BLIP_SPRITE(blip, spriteID);
	}

	if (name != nullptr)
	{
		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(name));
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
	}

	if (showRoute)
	{
		UI::SET_BLIP_ROUTE(blip, TRUE);
	}


	if (color != -1)
	{
		UI::SET_BLIP_COLOUR(blip, color);
	}

	return blip;
}

void MapBlip::RemoveBlip(Blip& blip)
{
	if (UI::DOES_BLIP_EXIST(blip))
	{
		UI::REMOVE_BLIP(&blip);		
	}

	blip = NULL;
}


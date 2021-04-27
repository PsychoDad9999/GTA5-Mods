// ----------------------------------------------------------------------------

#include "debugFrameOverlay.h"

#include <cstdio>

#include <inc/natives.h>
#include <framework/system/dateTime.h>
#include <framework/system/hashkey.h>
#include <framework/hud/text.h>
#include <framework/memory/memory.h>


// ----------------------------------------------------------------------------
/// <summery>Draw some ingame debug information</summery>
// ----------------------------------------------------------------------------
void DebugFrameOverlay::drawDebugOverlay(UINT64* pVar)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
		
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, TRUE); 

	char playerPosText[256];
	sprintf_s(playerPosText, "Player Pos: %07.1f/%07.1f/%07.1f", playerPos.x, playerPos.y, playerPos.z);

	Text::displayTextForCurrentFrame(playerPosText, 0.1, 0.2, true);


	
	BOOL scriptExist = SCRIPT::DOES_SCRIPT_EXIST("underwaterpickups");

	if (scriptExist == TRUE)
	{
		Text::displayTextForCurrentFrame("Script Exist", 0.1, 0.10, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("Script doesn't Exist", 0.1, 0.10, false);

	}

	BOOL scriptLoaded = SCRIPT::HAS_SCRIPT_LOADED("underwaterpickups");

	if (scriptLoaded == TRUE)
	{
		Text::displayTextForCurrentFrame("Script Loaded", 0.1, 0.15, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("Script not loaded", 0.1, 0.15, false);
	}


	char valueText[256];
	sprintf_s(valueText, "Game Version: %d", static_cast<int>(getGameVersion()));
	Text::displayTextForCurrentFrame(valueText, 0.1, 0.25, true);



	Text::displayTextForCurrentFrame("Time", 0.75, 0.10, true);


	DateTime dateTime = DateTime::getIngameTime();

	char currentIngameDateTimeText[256];
	sprintf_s(currentIngameDateTimeText, "%02d:%02d:%02d %02d.%02d.%02d [%#010x][Time]", dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond(), dateTime.getDay(), dateTime.getMonth(), dateTime.getYear(), dateTime.getRawData());

	Text::displayTextForCurrentFrame(currentIngameDateTimeText, 0.75, 0.15, true);



	int rawValue = 0;

	if (Memory::readInt32FromMemory(pVar, &rawValue, true, true))
	{
		DateTime spawnTime = DateTime(rawValue);

		char ct2[256];
		sprintf_s(ct2, "%02d:%02d:%02d %02d.%02d.%02d [%#010x][Spawn]", spawnTime.getHour(), spawnTime.getMinute(), spawnTime.getSecond(), spawnTime.getDay(), spawnTime.getMonth(), spawnTime.getYear(), spawnTime.getRawData());

		Text::displayTextForCurrentFrame(ct2, 0.75, 0.20, spawnTime.getRawData() != DateTime::TIME_NOT_SET);


		if (spawnTime.getRawData() != DateTime::TIME_NOT_SET)
		{
			int secondsLeft = DateTime::getTimeDifferenceInSeconds(dateTime, spawnTime);

			char ct3[256];
			sprintf_s(ct3, "Next Spawn in %d s", secondsLeft);

			Text::displayTextForCurrentFrame(ct3, 0.75, 0.25, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("Next spawn in [NOT AVAILABLE]", 0.75, 0.25, false);
		}		

		/*
		int timeCmp = dateTime.compareTo(spawnTime);

		char ct4[256];
		sprintf_s(ct4, "Time Cmp: %d", timeCmp);

		Text::displayTextForCurrentFrame(ct4, 0.75, 0.30, true);*/
	}
}

// ----------------------------------------------------------------------------
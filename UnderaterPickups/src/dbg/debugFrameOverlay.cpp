// ----------------------------------------------------------------------------

#include "debugFrameOverlay.h"

#include <cstdio>

#include "natives.h"
#include "framework/system/dateTime.h"
#include "framework/system/hashkey.h"
#include "framework/hud/text.h"

#include "time\timeHelper.h"


// ----------------------------------------------------------------------------
/// <summery>Draw some ingame debug information</summery>
// ----------------------------------------------------------------------------
void DebugFrameOverlay::drawDebugOverlay(const UINT64* pVar)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
		
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, TRUE); 

	char playerPosText[256];
	sprintf_s(playerPosText, "Player Pos: %07.1f/%07.1f/%07.1f", playerPos.x, playerPos.y, playerPos.z);

	Text::displayTextForCurrentFrame(playerPosText, 0.1, 0.2, true);

	
	if (HashKey::GetHashKey("pickup_money_case") == 0xCE6FDD6B)
	{
		Text::displayTextForCurrentFrame("Same Hash", 0.9, 0.10, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("Hash doesn't match", 0.9, 0.10, false);
	}

	
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


	/*
	if (pVar == nullptr)
	{
		Text::displayTextForCurrentFrame("Pointer is NULL", 0.1, 0.15, false);
	}
	else
	{
		Text::displayTextForCurrentFrame("Pointer is valid", 0.1, 0.15, true);
	}

	if (pVar != nullptr)
	{
		char valueText[256];
		sprintf_s(valueText, "Value: %lld", *pVar);
		Text::displayTextForCurrentFrame(valueText, 0.1, 0.20, true);

		Text::displayTextForCurrentFrame("Next HQ Spawn: Time not set", 0.1, 0.25, true);		
	}
	*/



	char currentIngameYearText[256];	
	sprintf_s(currentIngameYearText, "%d:%d:%d %d.%d.%d [N]", TIME::GET_CLOCK_HOURS(), TIME::GET_CLOCK_MINUTES(), TIME::GET_CLOCK_SECONDS(), TIME::GET_CLOCK_DAY_OF_MONTH(), TIME::GET_CLOCK_MONTH() + 1, TIME::GET_CLOCK_YEAR());
	Text::displayTextForCurrentFrame(currentIngameYearText, 0.9, 0.15, true);


	DateTime dateTime = TimeHelper::getIngameTime();

	char currentIngameDateTimeText[256];
	sprintf_s(currentIngameDateTimeText, "%d:%d:%d %d.%d.%d [I]", dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond(), dateTime.getDay(), dateTime.getMonth(), dateTime.getYear());


	Text::displayTextForCurrentFrame(currentIngameDateTimeText, 0.9, 0.20, true);




	dateTime = DateTime(DateTime::TIME_NOT_SET);

	char customIngameDateTimeText[256];
	sprintf_s(customIngameDateTimeText, "%d:%d:%d %d.%d.%d [C]", dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond(), dateTime.getDay(), dateTime.getMonth(), dateTime.getYear());


	Text::displayTextForCurrentFrame(customIngameDateTimeText, 0.9, 0.25, true);

/*
	if (oldVal == NOT_SET)
	{
		Text::displayTextForCurrentFrame("Not set [O]", 0.9, 0.30, false);
	}
	else
	{
		dateTime = DateTime(static_cast<UINT32>(oldVal));

		char oldSpawnDateTimeText[256];
		sprintf_s(oldSpawnDateTimeText, "%d:%d:%d %d.%d.%d [O]", dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond(), dateTime.getDay(), dateTime.getMonth(), dateTime.getYear());

		Text::displayTextForCurrentFrame(oldSpawnDateTimeText, 0.9, 0.30, true);
	}
	*/

}

// ----------------------------------------------------------------------------
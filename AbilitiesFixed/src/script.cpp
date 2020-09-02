// ----------------------------------------------------------------------------

#include "script.h"

#include "inc/natives.h"
#include "framework/system/dateTime.h"
#include "framework/system/hashKey.h"
#include "framework/hud/notifications.h"
#include "framework/hud/text.h"
#include "framework/diagnostics/debugConsole.h"
#include "framework/game/gameVersion.h"
#include "framework/game/globals.h"
#include "framework/world/player.h"
#include "framework/input/keyboard.h"
#include "framework/input/keydefines.h"
#include "framework/memory/memory.h"

#include "dbg/debugFrameOverlay.h"

#include <chrono>
#include <iostream>

// ----------------------------------------------------------------------------

//#define DEBUG_CONSOLE		   // opens debug console for usage of printf
//#define DEBUG_DRAW_OVERLAY   // draw status messages ingame 
//#define DEBUG_KEYS		   // use debug keys

// ----------------------------------------------------------------------------

Hash hashSpecialAbilityProgressPercentMicheal = HashKey::GetHashKey("SP0_SPECIAL_ABILITY_UNLOCKED");
Hash hashSpecialAbilityProgressPercentFranklin = HashKey::GetHashKey("SP1_SPECIAL_ABILITY_UNLOCKED");
Hash hashSpecialAbilityProgressPercentTrevor = HashKey::GetHashKey("SP2_SPECIAL_ABILITY_UNLOCKED");

Hash hashSpecialAbilityValueMicheal = HashKey::GetHashKey("SP0_SPECIAL_ABILITY");
Hash hashSpecialAbilityValueFranklin = HashKey::GetHashKey("SP1_SPECIAL_ABILITY");
Hash hashSpecialAbilityValueTrevor = HashKey::GetHashKey("SP2_SPECIAL_ABILITY");

// ----------------------------------------------------------------------------

const char* SPECIAL_ABILITY_PATTERN_MICHAEL  = "\xA8\x4C\xDD\xDC\x81\x9F\xCD\x4E";  //Michael
const char* SPECIAL_ABILITY_PATTERN_FRANKLIN = "\x76\x16\x15\x15\xA3\xFB\xCC\x51";  //Franklin
const char* SPECIAL_ABILITY_PATTERN_TREVOR   = "\xA3\xE7\x8F\x07\x42\x64\xB0\x05";  //Trevor
const char* SPECIAL_ABILITY_MASK = "xxxxxxxx";

// ----------------------------------------------------------------------------

bool g_patchApplied = false;

// ----------------------------------------------------------------------------
/// <summery>Normalize Value</summery>
/// <param name="value">Integer value</param>
/// <returns>Retruns a normalized value between 0-100</returns>
// ----------------------------------------------------------------------------
int normalizePercent(int value)
{
	if (value < 0)
		value = 0;

	if (value > 100)
		value = 100;

	return value;
}

// ----------------------------------------------------------------------------
/// <summery>Calculate special ability value from percentage value</summery>
/// <param name="value">percent</param>
/// <returns>Retruns a special ability value between 0-30</returns>
// ----------------------------------------------------------------------------
int getMaxSpecialAbilityValue(int progressPercent)
{	
	float maxValue = 30.f * normalizePercent(progressPercent) / 100.f;
	
	return static_cast<int>(round(maxValue));
}



bool getHashValuesForCharacter(ePlayerIdentifier playerIdentifier, Hash& hashAbilityValue, Hash& hashAbilityProgressPercent)
{
	switch (playerIdentifier)
	{
	case ePlayerIdentifier::MICHAEL:
		hashAbilityProgressPercent = hashSpecialAbilityProgressPercentMicheal;
		hashAbilityValue = hashSpecialAbilityValueMicheal;		
		break;

	case ePlayerIdentifier::FRANKLIN:
		hashAbilityProgressPercent = hashSpecialAbilityProgressPercentFranklin;
		hashAbilityValue = hashSpecialAbilityValueFranklin;		
		break;

	case ePlayerIdentifier::TREVOR:
		hashAbilityProgressPercent = hashSpecialAbilityProgressPercentTrevor;
		hashAbilityValue = hashSpecialAbilityValueTrevor;		
		break;

	default:
		return false;		
	}

	return true;
}


char* getSearchPatternForCharacter(ePlayerIdentifier playerIdentifier)
{
	switch (playerIdentifier)
	{
	case ePlayerIdentifier::MICHAEL:
		return (char*)SPECIAL_ABILITY_PATTERN_MICHAEL;

	case ePlayerIdentifier::FRANKLIN:
		return (char*)SPECIAL_ABILITY_PATTERN_FRANKLIN;

	case ePlayerIdentifier::TREVOR:
		return (char*)SPECIAL_ABILITY_PATTERN_TREVOR;

	default:
		return NULL;
	}
}



bool writeAbilityValueToMemory(UINT64* pAddress, float newAbilityValue)
{
	if (pAddress == nullptr)
		return false;

	// check input value
	if (newAbilityValue < 0.f || newAbilityValue > 30.f)
	{		
		return false; // ability value must be between 0-30
	}


	float currentAbilityValue = 0.0f;

	if (!Memory::readFloatFromMemory(pAddress, &currentAbilityValue, false))
	{
		printf("Failed to read float from memory location\n");
		return false;
	}

	// make some checks to make sure we are reading from the correct memory location
	if (currentAbilityValue < 0.f || currentAbilityValue > 30.f)
	{
		printf("Invalid value range\n");
		return false; // values should be between 0-30
	}

	if (Memory::writeFloatToMemory(pAddress, static_cast<float>(newAbilityValue), false))
	{
		return true;
	}
	else
	{
		printf("Failed to write float to memory location\n");
		return false;
	}
}



bool fixLoadingGlitch(Ped playerPed)
{
	Hash playerModelHash = ENTITY::GET_ENTITY_MODEL(playerPed);

	if (PLAYER::IS_SPECIAL_ABILITY_UNLOCKED(playerModelHash))
	{
		Hash hashAbilityProgressPercent = 0;		
		Hash hashAbilityValue = 0;

		ePlayerIdentifier playerIdentifier = PlayerIdentifier::getPlayerIdentifierFromModelHash(playerModelHash);

		if (!getHashValuesForCharacter(playerIdentifier, hashAbilityValue, hashAbilityProgressPercent))
			return false;

		// percent of Special Ability
		int abilityProgressPercent = 0;

		if (!STATS::STAT_GET_INT(hashAbilityProgressPercent, &abilityProgressPercent, -1))
		{
			printf("STATS::STAT_GET_INT -> Failed to get Special Ability Progress\n");
			return false;
		}

		int targetPercent = abilityProgressPercent;
	
		int maxTargetSpecialAbilityValue = getMaxSpecialAbilityValue(targetPercent);
		int abilityValue = 0;

		if (!STATS::STAT_GET_INT(hashAbilityValue, &abilityValue, -1))
		{
			printf("STATS::STAT_GET_INT -> Failed to get Special Ability Value\n");
			return false;
		}

		if (abilityValue == maxTargetSpecialAbilityValue)
		{
			return true; // nothing to fix
		}

		// get memory search pattern
		char* searchPattern = getSearchPatternForCharacter(playerIdentifier);

		if (searchPattern == NULL)
		{
			printf("Memory search pattern empty\n");
			return false;
		}

		// this search can take some time (a couple of seconds)
		UINT64* pSpecialAbilityMemoryBaseLocation = Memory::findPattern(searchPattern, (char*)SPECIAL_ABILITY_MASK);

		if (pSpecialAbilityMemoryBaseLocation == nullptr)
		{
			printf("Memory pattern not found\n");
			return false;
		}

		// add offset
		UINT64* pSpecialAbilityMemoryLocation = pSpecialAbilityMemoryBaseLocation + 5;
		UINT64* pMaxSpecialAbilityMemoryLocation = pSpecialAbilityMemoryBaseLocation + 6;

		bool isBugFixed = writeAbilityValueToMemory(pMaxSpecialAbilityMemoryLocation, static_cast<float>(maxTargetSpecialAbilityValue));

		if (isBugFixed)
		{
			// Fill the ability bar
			writeAbilityValueToMemory(pSpecialAbilityMemoryLocation, static_cast<float>(maxTargetSpecialAbilityValue));
		}

		return isBugFixed;
	}
	else
	{
		// nothing to fix
		printf("Nothing to fix. Special Ability not unlocked\n");
		return true;
	}	
}


void update()
{	
	if (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
		return;

	// Update Notifications
	Notifications::update();
	
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		return;

	if (!PLAYER::IS_PLAYER_PLAYING(player))
		return;
					
	if (!g_patchApplied)
	{		
		// call this once
		g_patchApplied = true;

		if (!fixLoadingGlitch(playerPed))			
		{
			Notifications::update();
			Notifications::showNotification("Failed to fix special ability glitch.", 5000, 1);
		}	
	}
	
	#ifdef DEBUG_KEYS

	if (Keyboard::IsKeyJustDown(KEY_O))
	{
		PLAYER::SPECIAL_ABILITY_DEPLETE_METER(PLAYER::PLAYER_ID(), TRUE);
	}

	if (Keyboard::IsKeyJustDown(KEY_L))
	{
		PLAYER::SPECIAL_ABILITY_CHARGE_ABSOLUTE(PLAYER::PLAYER_ID(), 30, TRUE);
	}
	
	#endif

	#ifdef DEBUG_DRAW_OVERLAY
	// draw debug overlay
	DebugFrameOverlay::drawDebugOverlay(playerPed);
	#endif
}


void initScript()
{
	#ifdef DEBUG_CONSOLE
	// create debug console
	DebugConsole::init();
	#endif		
}



void runScript()
{	
	// Reset this to false here. This Method is called when a savegame is loaded
	g_patchApplied = false;

	while (true)
	{
		update();
		WAIT(0);
	}
}


void cleanupScript()
{
	#ifdef DEBUG_CONSOLE
	// dispose debug console
	DebugConsole::dispose();
	#endif	
}
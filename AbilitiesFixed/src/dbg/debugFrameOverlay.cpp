// ----------------------------------------------------------------------------

#include "debugFrameOverlay.h"

#include <cstdio>
#include <chrono>
#include <algorithm>
#include <vector>

#include "inc/natives.h"
#include "framework/system/dateTime.h"
#include "framework/system/hashkey.h"
#include "framework/hud/text.h"
#include "framework/world/player.h"


typedef std::chrono::duration<int, std::ratio<86400>> Days;

// ----------------------------------------------------------------------------
/// <summery>Draw some ingame debug information</summery>
// ----------------------------------------------------------------------------
void DebugFrameOverlay::drawDebugOverlay(Ped playerPed)
{
	Hash playerModelHash = ENTITY::GET_ENTITY_MODEL(playerPed);
	
	Hash hashAbility = 0;
	Hash hashAbilityMaxed = 0;
	Hash hashAbilityUnlocked = 0;

	ePlayerIdentifier player = PlayerIdentifier::getPlayerIdentifierFromModelHash(playerModelHash);

	switch (player)
	{
	case ePlayerIdentifier::MICHAEL:
		Text::displayTextForCurrentFrame("Micheal", 0.1, 0.05, true);

		hashAbility = HashKey::GetHashKey("SP0_SPECIAL_ABILITY");
		hashAbilityMaxed = HashKey::GetHashKey("SP0_SPECIAL_ABILITY_MAXED");
		hashAbilityUnlocked = HashKey::GetHashKey("SP0_SPECIAL_ABILITY_UNLOCKED");
		break;

	case ePlayerIdentifier::FRANKLIN:
		Text::displayTextForCurrentFrame("Franklin", 0.1, 0.05, true);

		hashAbility = HashKey::GetHashKey("SP1_SPECIAL_ABILITY");
		hashAbilityMaxed = HashKey::GetHashKey("SP1_SPECIAL_ABILITY_MAXED");
		hashAbilityUnlocked = HashKey::GetHashKey("SP1_SPECIAL_ABILITY_UNLOCKED");
		break;

	case ePlayerIdentifier::TREVOR:
		Text::displayTextForCurrentFrame("Trevor", 0.1, 0.05, true);

		hashAbility = HashKey::GetHashKey("SP2_SPECIAL_ABILITY");
		hashAbilityMaxed = HashKey::GetHashKey("SP2_SPECIAL_ABILITY_MAXED");
		hashAbilityUnlocked = HashKey::GetHashKey("SP2_SPECIAL_ABILITY_UNLOCKED");
		break;

	default:
		Text::displayTextForCurrentFrame("Unknown", 0.1, 0.05, false);
		break;
	}


	if (hashAbility != 0)
	{
		int abilityValue = 0;
		if (STATS::STAT_GET_INT(hashAbility, &abilityValue, -1))
		{
			char abilityValueText[256];
			sprintf_s(abilityValueText, "Special: %d", abilityValue);

			Text::displayTextForCurrentFrame(abilityValueText, 0.1, 0.1, true);
		}
	}

	if (hashAbilityUnlocked != 0)
	{
		int abilityUnlockedValue = 0;
		if (STATS::STAT_GET_INT(hashAbilityUnlocked, &abilityUnlockedValue, -1))
		{
			char abilityUnlockedValueText[256];
			sprintf_s(abilityUnlockedValueText, "Progress: %d%%", abilityUnlockedValue);

			Text::displayTextForCurrentFrame(abilityUnlockedValueText, 0.1, 0.15, true);
		}
	}

	if (hashAbilityMaxed != 0)
	{
		int abilityMaxedValue = 0;
		if (STATS::STAT_GET_INT(hashAbilityMaxed, &abilityMaxedValue, -1))
		{
			if (abilityMaxedValue == 0)
			{
				// ability has not maxed out yet
				Text::displayTextForCurrentFrame("Maxed duration: N/A", 0.1, 0.20, false);
			}
			else
			{
				std::chrono::milliseconds duration(abilityMaxedValue);

				Days days = std::chrono::duration_cast<Days>(duration);
				duration -= days;
				auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
				duration -= hours;
				auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
				duration -= minutes;
				auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
				duration -= seconds;

				char abilityMaxedValueText[256];
				sprintf_s(abilityMaxedValueText, "Maxed: %dd %dh %dm %I64ds", days.count(), hours.count(), minutes.count(), seconds.count());
				Text::displayTextForCurrentFrame(abilityMaxedValueText, 0.1, 0.20, true);
			}
		}
	}


	BOOL isUnlocked = PLAYER::IS_SPECIAL_ABILITY_UNLOCKED(playerModelHash);
	if (isUnlocked)
	{
		Text::displayTextForCurrentFrame("IsUnlocked: True", 0.1, 0.25, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("IsUnlocked: False", 0.1, 0.25, false);
	}


	BOOL isMeterFull = PLAYER::IS_SPECIAL_ABILITY_METER_FULL(PLAYER::GET_PLAYER_INDEX());
	if (isMeterFull)
	{
		Text::displayTextForCurrentFrame("IsFull: True", 0.1, 0.30, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("IsFull: False", 0.1, 0.30, false);
	}


	BOOL isActive = PLAYER::IS_SPECIAL_ABILITY_ACTIVE(PLAYER::GET_PLAYER_INDEX());
	if (isActive)
	{
		Text::displayTextForCurrentFrame("IsActive: True", 0.1, 0.35, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("IsActive: False", 0.1, 0.35, false);
	}



	BOOL initialized = FALSE;
	STATS::STAT_GET_BOOL(HashKey::GetHashKey("CLO_STORED_INITIAL"), &initialized, TRUE);
	if (initialized)
	{
		Text::displayTextForCurrentFrame("Initalized: True", 0.1, 0.40, true);
	}
	else
	{
		Text::displayTextForCurrentFrame("Initalized: False", 0.1, 0.40, false);
	}
}

// ----------------------------------------------------------------------------

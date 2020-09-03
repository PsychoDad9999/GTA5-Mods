// ----------------------------------------------------------------------------

#include "script.h"

#include <inc/natives.h>
#include <framework/system/dateTime.h>
#include <framework/system/hashKey.h>
#include <framework/hud/notifications.h>
#include <framework/hud/text.h>
#include <framework/diagnostics/debugConsole.h>
#include <framework/game/gameVersion.h>
#include <framework/game/globals.h>
#include <framework/world/world.h>

#include "settings/settings.h"
#include "dbg/debugFrameOverlay.h"
#include "package/hiddenPackagesManager.h"

#include <chrono>
#include <iostream>

// ----------------------------------------------------------------------------

//#define DEBUG_CONSOLE			// opens debug console for usage of printf
//#define DEBUG_DRAW_OVERLAY		// draw status messages ingame 

// ----------------------------------------------------------------------------

Settings::ModSettings g_settings;

// Global Game Variable
UINT64* g_pNextSpawnDateTime;

HiddenPackagesManager g_packagesManager;

// check if GTA UnderwaterPickups script was running last frame
bool g_isScriptRunning = false;
bool g_unsupportedGameVersionErrorShown = false;

const int NOTIFICATION_RESPAWN = 0;
const int NOTIFICATION_VERSION = 1;

// ----------------------------------------------------------------------------

static const Hash SCRIPT_HASH_UNDERWATER_PICKUPS = HashKey::GetHashKey("underwaterpickups");
static const Hash HASH_MONEY_CASE_PICKUP = HashKey::GetHashKey("pickup_money_case");

void showNotification(char* message, int id);

// ----------------------------------------------------------------------------


void handleInstantRespawn()
{
	// Set next spawn time of underwater pickups
	if (*g_pNextSpawnDateTime != DateTime::TIME_NOT_SET)
	{
		*g_pNextSpawnDateTime = DateTime::TIME_NOT_SET;

		showNotification("Nearby underwater pickup has respawned.", NOTIFICATION_RESPAWN);
	}
}



void handleDelayedRespawn(Vector3 playerLocation)
{
	double scriptActivationDistance;

	// get closest package spawn area relativ to player position
	HiddenPackage* closestPackage = g_packagesManager.getPackageWithClosestActivationPoint(playerLocation, &scriptActivationDistance);


	// overwrite global variable with value of closest package
	if (*g_pNextSpawnDateTime != closestPackage->nextSpawnTime.getRawData())
	{
		if (*g_pNextSpawnDateTime == DateTime::TIME_NOT_SET)
		{
			// clear saved timer values
			g_packagesManager.purge();
		}


		*g_pNextSpawnDateTime = closestPackage->nextSpawnTime.getRawData();
	}


	if (scriptActivationDistance <= 250) // GTA UnderwaterPickup-Script will be activated at ~210m.
	{	
		bool isScriptRunning = SCRIPT::_DOES_SCRIPT_WITH_NAME_HASH_EXIST(SCRIPT_HASH_UNDERWATER_PICKUPS) == TRUE && SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(SCRIPT_HASH_UNDERWATER_PICKUPS) == TRUE;

		if (!g_isScriptRunning && isScriptRunning)
		{
			DateTime currentIngameTime = DateTime::getIngameTime();			

			// Script changed from not running to running
			if (DateTime::compare(currentIngameTime, closestPackage->nextSpawnTime) > 0)
			{
				showNotification("Nearby underwater pickup has respawned.", NOTIFICATION_RESPAWN);
			}
			else
			{
				char nextRespawnTimeText[256];

				int minutesLeft = DateTime::getTimeDifferenceInSeconds(currentIngameTime, closestPackage->nextSpawnTime) / 60;
				
				if (minutesLeft <= 120)
				{
					sprintf_s(nextRespawnTimeText, "Nearby underwater pickup will respawn in %d minutes.", minutesLeft);
				}
				else
				{
					sprintf_s(nextRespawnTimeText, "Nearby underwater pickup will respawn in %d hours.", minutesLeft / 60);
				}							   						

				showNotification(nextRespawnTimeText, NOTIFICATION_RESPAWN);
			}
		}

		g_isScriptRunning = isScriptRunning;

		if (isScriptRunning)
		{
			double distanceToPackageSpawn = World::getDistance(closestPackage->packageLocation, playerLocation, false);

			if (distanceToPackageSpawn < 20)  // 20m radius, track package pickup
			{
				BOOL isPickupInRange = OBJECT::_IS_PICKUP_WITHIN_RADIUS(HASH_MONEY_CASE_PICKUP, closestPackage->packageLocation.x, closestPackage->packageLocation.y, closestPackage->packageLocation.z, 5);

				if (closestPackage->isPickupInRange && !isPickupInRange)
				{
					DateTime nextSpawnTime = DateTime::getIngameTime();
					nextSpawnTime.addTime(g_settings.behavior.respawnTimeInHours, 0, 0, 0, 0, 0);

					closestPackage->nextSpawnTime = nextSpawnTime;
					*g_pNextSpawnDateTime = nextSpawnTime.getRawData();
				}

				closestPackage->isPickupInRange = isPickupInRange;				
			}
			else
			{
				closestPackage->isPickupInRange = false;
			}
		}
	}
	else
	{
		g_isScriptRunning = false;
		closestPackage->isPickupInRange = false;
	}
}



void showNotification(char* message, int id)
{
	if (g_settings.hud.showRespawnNotification)
	{
		Notifications::showNotification(message, g_settings.hud.notificationDuration, id);
	}
}


void update()
{
	// Update Notifications
	Notifications::update();

	if (g_pNextSpawnDateTime == nullptr && !DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
	{
		if (g_settings.hud.showUnsupportedVersionNotification && !g_unsupportedGameVersionErrorShown)
		{
			// show only once
			g_unsupportedGameVersionErrorShown = true;
			Notifications::showNotification("UnderwaterPickups.asi\nGame version not supported. Please check for an update", 5000, NOTIFICATION_VERSION);
		}
	}
		
	#ifdef DEBUG_DRAW_OVERLAY
	// draw debug overlay
	DebugFrameOverlay::drawDebugOverlay(g_pNextSpawnDateTime);
	#endif

	// Game version not supported
	if (g_pNextSpawnDateTime == nullptr)
		return;
	
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		return;


	if (g_settings.behavior.respawnTimeInHours == 0)
	{
		handleInstantRespawn();
	}
	else
	{
		handleDelayedRespawn(World::getPosition(playerPed));
	}	
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
	// load settings from ini file
	Settings::loadFromFile(g_settings, TEXT("UnderwaterPickups.ini"));

	g_pNextSpawnDateTime = Globals::getGlobal(eGlobals::UNDERWATER_PICKUP_RESPAWN_TIME, static_cast<eExtGameVersion>(getGameVersion()));
	g_isScriptRunning = false;

	if (g_settings.hud.showPackageBlips)
	{
		g_packagesManager.createPackageBlips();
	}

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
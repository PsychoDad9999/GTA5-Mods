// ----------------------------------------------------------------------------

#include "script.h"

#include "inc/natives.h"
#include "framework/hud/notifications.h"
#include "framework/hud/helpMessages.h"
#include "framework/input/keyboard.h"
#include "framework/input/keydefines.h"
#include "framework/diagnostics/debugConsole.h"
#include "framework/audio/audioPlayer.h"
#include "framework/world/world.h"
#include "framework/world/model.h"
#include "framework/world/rope.h"
#include "framework/system/hashKey.h"

#include "hud/blip.h"
#include "settings/settings.h"
#include "vehicle/anchoredVehicleManager.h"
#include "vehicle/vehicleEntity.h"
#include "input/controls.h"
#include "dbg/debugFrameOverlay.h"
#include "dbg/debugVehicleSpawner.h"
#include "resource.h"

// ----------------------------------------------------------------------------

//#define DEBUG_CONSOLE				// opens debug console for usage of printf
//#define DEBUG_DRAW_OVERLAY		    // draw status messages ingame 
//#define DEBUG_VEHICLE_SPAWN			// press F12 to spawn a vehicle

// ----------------------------------------------------------------------------

const int NOTIFICATION_ANCHORED = 1;

const int INPUTGROUP_MOVE = 0;
const int INPUT_ENTER = 23;

// ----------------------------------------------------------------------------

Settings::ModSettings g_settings;
AnchoredVehicleManager g_vehicleManager;
ControlsWatcher g_enterSubControlsWatcher = ControlsWatcher(INPUTGROUP_MOVE, INPUT_ENTER, 1500);
bool g_helpMessageHoldKeyShown = false;

// ----------------------------------------------------------------------------

bool canAnchorInCurrentSituation(AnchoredVehicle* const vehicleWithAnchor);
void setAnchorAndBlip(AnchoredVehicle* const vehicleWithAnchor);
void removeAnchorAndBlip(AnchoredVehicle* const vehicleWithAnchor, bool playSound);
void playAudio(eVehicleType vehicle, bool isAnchorDirectionDown);

// ----------------------------------------------------------------------------

void handlePlayerOnBoat(Ped playerPed, bool isPlayerInAnySub)
{
	// Get current vehicle
	Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

	if (vehicle != NULL)
	{
		const eVehicleType vehicleType = VehicleEntity::getVehicleType(vehicle, isPlayerInAnySub);
	
		if(g_settings.isVehicleTypeAllow(vehicleType))		
		{
			// Check if vehicle has an anchor and anchor is available
			// This can return false when the boat is on land or very close to an obstacle
			// or the submersible is too close to the sea bottom
			if (VEHICLE::_GET_BOAT_ANCHOR(vehicle))
			{			
				// Check if this is a known vehicle
				if (!g_vehicleManager.contains(vehicle))
				{					
					// Add new boat to Boat Manager
					g_vehicleManager.add(vehicle, vehicleType);
				}				

				AnchoredVehicle* vehicleWithAnchor = g_vehicleManager.get(vehicle);

				if (vehicleWithAnchor != nullptr)
				{
					if (vehicleWithAnchor->isAnchored)
					{
						if (!vehicleWithAnchor->isSecured && !vehicleWithAnchor->isReentered) // Boat has been anchored but not yet secured. Slow down boat
						{	
							if (vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypePlane || vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypeHeli) // BRING_VEHICLE_TO_HALT doesn't work on planes
							{									
								if (!vehicleWithAnchor->breakingData.isInitialized)
								{
									// Initialize breaking process
									double planeSpeedKmph = VehicleEntity::getVehicleSpeedKmph(vehicleWithAnchor->vehicle);

									if (VehicleEntity::isDrivingBackwards(vehicleWithAnchor->vehicle))
										planeSpeedKmph *= -1;

									double breakingDistanceInMeters = Physics::calculateEmergencyBreakingDistanceInMeters(planeSpeedKmph);

									vehicleWithAnchor->breakingData.totalTimeToStop = Physics::calculateTotalTimeOfBreakingInMilliseconds(planeSpeedKmph, breakingDistanceInMeters);
									vehicleWithAnchor->breakingData.initialSpeed = planeSpeedKmph;
									vehicleWithAnchor->breakingData.breakingStartTime = GetTickCount64();
									vehicleWithAnchor->breakingData.breakingDistanceInMeters = breakingDistanceInMeters;
									vehicleWithAnchor->breakingData.hasStopped = false;
									vehicleWithAnchor->breakingData.isInitialized = true;
								}
								else
								{									
									if (vehicleWithAnchor->breakingData.hasStopped)
									{
										// keep in place
										if (ENTITY::IS_ENTITY_IN_WATER(vehicleWithAnchor->vehicle))
										{
											VEHICLE::BRING_VEHICLE_TO_HALT(vehicleWithAnchor->vehicle, 0.0, 1, TRUE);
										}
									}
									else
									{
										ULONGLONG currentTime = GetTickCount64();

										if (currentTime - vehicleWithAnchor->breakingData.breakingStartTime >= vehicleWithAnchor->breakingData.totalTimeToStop)
										{
											// .. breaking process completed
											vehicleWithAnchor->breakingData.hasStopped = true;

											// do this only once, otherwise wave physics will break.
											VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicleWithAnchor->vehicle);																						
										}
										else
										{
											// .. in breaking process

											ULONGLONG totalTimeToStop = vehicleWithAnchor->breakingData.totalTimeToStop <= 0 ? 1 : vehicleWithAnchor->breakingData.totalTimeToStop; // to avoid divide-by-zero-exception

											double percentBreakingComplete = ((currentTime - vehicleWithAnchor->breakingData.breakingStartTime) * 100.f) / totalTimeToStop; 

											double newSpeedKmph = vehicleWithAnchor->breakingData.initialSpeed - ((vehicleWithAnchor->breakingData.initialSpeed * percentBreakingComplete) / 100.f);

											VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicleWithAnchor->vehicle, static_cast<float>(Physics::kmphToMps(newSpeedKmph)));
										}
									}
								}								
							}
							else // Vehicle is not a plane
							{								
								if (VehicleEntity::getVehicleSpeedKmph(vehicle) > 30) // faster than 30 km/h
								{
									VEHICLE::BRING_VEHICLE_TO_HALT(vehicleWithAnchor->vehicle, 10.0, 1, TRUE);
								}
								else
								{
									VEHICLE::BRING_VEHICLE_TO_HALT(vehicleWithAnchor->vehicle, 3.0, 1, TRUE);
								}																
							}
						}
						else  // Boat was anchored and secured and has been reentered
						{
							vehicleWithAnchor->isReentered = true;

							// vehicle is no longer secured and must be resecured in case player leave the vehicle
							vehicleWithAnchor->isSecured = false;

							if (!g_settings.behavior.autoRetrieveAnchorWhenGetInVehicle)
							{
								// Hold vehicle in position until anchor key is pressed
								//VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicleWithAnchor->vehicle, 0);
								VEHICLE::BRING_VEHICLE_TO_HALT(vehicleWithAnchor->vehicle, 0.0, 1, TRUE);
							}
							else
							{
								// backup vehicle ID
								Vehicle id = vehicleWithAnchor->vehicle;
 
								// remove anchor and map blip
								removeAnchorAndBlip(vehicleWithAnchor, true);

								g_vehicleManager.remove(id);
								vehicleWithAnchor = nullptr;
							}
						}
					}

					if (vehicleWithAnchor != nullptr) 
					{
						// Manual retrieve or drop anchor 
						if(Keyboard::IsValidKey(g_settings.input.anchorKey) && Keyboard::IsKeyJustDown(g_settings.input.anchorKey))
						{
							if (vehicleWithAnchor->isAnchored && vehicleWithAnchor->isReentered)
							{
								// backup vehicle ID
								Vehicle id = vehicleWithAnchor->vehicle;

								// remove anchor and map blip
								removeAnchorAndBlip(vehicleWithAnchor, true);

								g_vehicleManager.remove(id);
								vehicleWithAnchor = nullptr;
							}				


							if (vehicleWithAnchor != nullptr )
							{
								if (vehicleWithAnchor->isAnchored || (!vehicleWithAnchor->isAnchored && canAnchorInCurrentSituation(vehicleWithAnchor)))
								{
									playAudio(vehicleWithAnchor->vehicleType, !vehicleWithAnchor->isAnchored);

									vehicleWithAnchor->isAnchored = !vehicleWithAnchor->isAnchored;
									vehicleWithAnchor->isSecured = false;
									vehicleWithAnchor->isReentered = false;
									vehicleWithAnchor->breakingData = AnchoredVehicle::BreakingData();
								}
							}
						}
					}
				}
			}
			else
			{
				// show Anchor not available notification
			}
		}
	}		 
}




void handlePlayerOutsideBoat(Ped playerPed)
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		// Player is not in any vehicle. Get last vehicle
		Vehicle lastVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, TRUE);

		if (lastVehicle != NULL)
		{
			// Get Boat from Boat Manager
			AnchoredVehicle* const vehicleWithAnchor = g_vehicleManager.get(lastVehicle);

			if (vehicleWithAnchor != nullptr)
			{
				// check if boat is already secured
				if ((vehicleWithAnchor->isAnchored || g_settings.behavior.autoDeployAnchorWhenLeaveVehicle) && !vehicleWithAnchor->isSecured)
				{					
					setAnchorAndBlip(vehicleWithAnchor);

					if (!vehicleWithAnchor->isAnchored) // Play anchor sound
					{						
						playAudio(vehicleWithAnchor->vehicleType, true);
					}

					vehicleWithAnchor->isAnchored = true;
					vehicleWithAnchor->isSecured = true;
					vehicleWithAnchor->isReentered = false;
					vehicleWithAnchor->blipPosition = World::getPosition(vehicleWithAnchor->vehicle);
					vehicleWithAnchor->breakingData = AnchoredVehicle::BreakingData();

					// show helpText
					if (vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypeSub)
					{
						if (g_settings.hud.allowHelpMessages && !g_helpMessageHoldKeyShown && g_settings.misc.allowHoldKeyToEnterSubmersible)
						{							
							if (!HelpMessages::isHelpMessageBeingDisplayed())
							{
								HelpMessages::showHelpMessage("Hold ~INPUT_ENTER~ to get back in the submersible.", -1, true);
								g_helpMessageHoldKeyShown = true;
							}
						}
					}
				}
				else if (!vehicleWithAnchor->isAnchored)
				{		
					// we can remove the boat from the BoatManager, if the boat is not anchored and we are not in the vehicle
					g_vehicleManager.remove(vehicleWithAnchor->vehicle);					
				}				
			}
		}


		if (g_settings.misc.allowHoldKeyToEnterSubmersible && g_enterSubControlsWatcher.isHoldDown())
		{
			AnchoredVehicle* const nearestSubmersible = g_vehicleManager.findNearestSecuredVehicle(World::getPosition(playerPed), 15, eVehicleType::VehicleTypeSub);

			if (nearestSubmersible != nullptr && nearestSubmersible->vehicle != 0)
			{
				// teleport player into vehicle				
				PED::SET_PED_INTO_VEHICLE(playerPed, nearestSubmersible->vehicle, -1);
							
				g_enterSubControlsWatcher.setIsHandled();
			}
		}		
	}
}



bool canAnchorInCurrentSituation(AnchoredVehicle* const vehicleWithAnchor)
{
	if (vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypePlane || vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypeHeli)
	{
		// check if plane or heli is in water
		return ENTITY::IS_ENTITY_IN_WATER(vehicleWithAnchor->vehicle);		
	}
	else
	{
		return true;
	}
}


void handleEntityDistanceRemoval(Ped playerPed)
{
	if (g_settings.behavior.anchorAutoRemovalDistance > 0)
	{
		Vector3 playerPos = World::getPosition(playerPed);

		AnchoredVehicle* const vehicleAtRemovalDistance = g_vehicleManager.findFirstSecuredVehicle(playerPos, g_settings.behavior.anchorAutoRemovalDistance);

		if (vehicleAtRemovalDistance != nullptr)
		{
			// remove anchor and map blip
			removeAnchorAndBlip(vehicleAtRemovalDistance, false);

			// remove from Boat Manager
			g_vehicleManager.remove(vehicleAtRemovalDistance->vehicle);
		}
	}
}


Entity createProp(Hash modelHash, Vector3 position, bool isStatic, bool placeOnGround)
{
	// load model, 1s timeout
	if (!Model::requestModel(modelHash, 1000))
		return 0;
	
	return OBJECT::CREATE_OBJECT(modelHash, position.x, position.y, placeOnGround ? World::getGroundHeight(position) : position.z, TRUE, TRUE, isStatic ? FALSE : TRUE);
}


Vector3 getVehicleAttachmentPoint(Vehicle vehicle)
{
	const Hash modelHash = ENTITY::GET_ENTITY_MODEL(vehicle);

	switch (modelHash)
	{		
	case 0xC07107EE: // Kraken
		return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 0, -1.0, -0.8);

	case 0x2DFF622F: //Submersible
		return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 0, -4.7, -1.35);

	default:
		return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 0, 0, 0);		
	}
}


void attachRopeToSubmersible(AnchoredVehicle* const vehicleWithAnchor)
{
	if (vehicleWithAnchor->vehicle != 0)
	{		
		Vector3 sourcePos = getVehicleAttachmentPoint(vehicleWithAnchor->vehicle);

		Entity sourceEntity = vehicleWithAnchor->vehicle;
		Entity anchorEntity = createProp(HashKey::GetHashKey("prop_ashtray_01"), sourcePos, true, true);

		if (anchorEntity != 0)
		{
			ENTITY::SET_ENTITY_VISIBLE(anchorEntity, FALSE, 0);
			ENTITY::FREEZE_ENTITY_POSITION(anchorEntity, TRUE);

			Vector3 anchorPos = World::getPosition(anchorEntity);

			float ropeLength = static_cast<float>(World::getDistance(sourcePos, anchorPos, false));
			float minRopeLength = max(0, ropeLength - 20);

			Object ropeHandle = Rope::createRope(eRopeType::THICK_ROPE, sourcePos, ropeLength, minRopeLength, false);

			if (ropeHandle != 0)
			{				
				Rope::attachEntities(ropeHandle, sourceEntity, sourcePos, anchorEntity, anchorPos, ropeLength);
				Rope::enablePhysics(ropeHandle);				
			}

			// save handles
			vehicleWithAnchor->ropeData.anchorEntity = anchorEntity;
			vehicleWithAnchor->ropeData.ropeHandle = ropeHandle;
		}		
	}
}



void removeRopeFromSubmersible(AnchoredVehicle* const vehicleWithAnchor)
{
	if (vehicleWithAnchor->vehicle != 0)
	{
		// delete rope
		if (vehicleWithAnchor->ropeData.ropeHandle != 0)
		{
			Rope::deleteRope(vehicleWithAnchor->ropeData.ropeHandle);
			vehicleWithAnchor->ropeData.ropeHandle = 0;
		}

		// delete anchor
		if (vehicleWithAnchor->ropeData.anchorEntity != 0)
		{
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehicleWithAnchor->ropeData.anchorEntity);
			vehicleWithAnchor->ropeData.anchorEntity = 0;
		}
	}
}



void setAnchorAndBlip(AnchoredVehicle* const vehicleWithAnchor)
{
	VEHICLE::SET_BOAT_ANCHOR(vehicleWithAnchor->vehicle, TRUE);

	MapBlip::add(vehicleWithAnchor->vehicle, vehicleWithAnchor->vehicleType, g_settings.hud.showAnchoredVehiclesOnMap, g_settings.hud.useShortRangeMapMarkers);

	if (vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypeSub)
	{
		// remove old rope
		if (vehicleWithAnchor->ropeData.ropeHandle != 0)
		{
			removeRopeFromSubmersible(vehicleWithAnchor);
		}

		// attach rope
		attachRopeToSubmersible(vehicleWithAnchor);
	}
		
	if (g_settings.hud.allowNotifications)
	{
		ULONG duration = g_settings.hud.notificationDuration;

		switch (vehicleWithAnchor->vehicleType)
		{
		case eVehicleType::VehicleTypeBoat:
			Notifications::showNotification("Boat anchored", duration, NOTIFICATION_ANCHORED);
			break;

		case eVehicleType::VehicleTypeJetSki:
			Notifications::showNotification("Jet Ski anchored", duration, NOTIFICATION_ANCHORED);
			break;

		case eVehicleType::VehicleTypeSub:
			Notifications::showNotification("Submersible anchored", duration, NOTIFICATION_ANCHORED);
			break;

		case eVehicleType::VehicleTypePlane:
			Notifications::showNotification("Seaplane anchored", duration, NOTIFICATION_ANCHORED);
			break;

		case eVehicleType::VehicleTypeHeli:
			Notifications::showNotification("Helicopter anchored", duration, NOTIFICATION_ANCHORED);
			break;

		default:
		case eVehicleType::VehicleTypeUnknown:
			Notifications::showNotification("Vehicle anchored", duration, NOTIFICATION_ANCHORED);
			break;
		}		
	}
}


void removeAnchorAndBlip(AnchoredVehicle* const vehicleWithAnchor, bool playSound)
{	
	VEHICLE::SET_BOAT_ANCHOR(vehicleWithAnchor->vehicle, FALSE);

	MapBlip::remove(vehicleWithAnchor->vehicle);

	if (playSound)
	{
		playAudio(vehicleWithAnchor->vehicleType, false);
	}		

	if (vehicleWithAnchor->vehicleType == eVehicleType::VehicleTypeSub)
	{
		// remove attached rope
		removeRopeFromSubmersible(vehicleWithAnchor);
	}
}


// ----------------------------------------------------------------------------
/// <summery>Play vehicle anchor sound</summery>
/// <param name="vehicle">Vehicle type</param>
/// <param name="isAnchorDirectionDown">true to use anchor-down sound effect, false to use anchor-up sound effect</param>
// ----------------------------------------------------------------------------
void playAudio(eVehicleType vehicle, bool isAnchorDirectionDown)
{
	if (g_settings.audio.allowAnchorSounds)
	{
		int resourceID;
		int volume = 100;

		switch (vehicle)
		{
		case eVehicleType::VehicleTypeJetSki:
			resourceID = isAnchorDirectionDown ? IDR_WAVE_JETSKI_ANCHOR_DOWN : IDR_WAVE_JETSKI_ANCHOR_UP;
			volume = g_settings.audio.volumeAnchorJetski;
			break;

		case eVehicleType::VehicleTypePlane:
			resourceID = isAnchorDirectionDown ? IDR_WAVE_PLANE_ANCHOR_DOWN : IDR_WAVE_PLANE_ANCHOR_UP;
			volume = g_settings.audio.volumeAnchorSeaPlane;
			break;

		case eVehicleType::VehicleTypeBoat:
			resourceID = isAnchorDirectionDown ? IDR_WAVE_BOAT_ANCHOR_DOWN : IDR_WAVE_BOAT_ANCHOR_UP;
			volume = g_settings.audio.volumeAnchorBoat;
			break;

		case eVehicleType::VehicleTypeSub:
			resourceID = isAnchorDirectionDown ? IDR_WAVE_SUB_ANCHOR_DOWN : IDR_WAVE_SUB_ANCHOR_UP;
			volume = g_settings.audio.volumeAnchorSub;
			break;

		case eVehicleType::VehicleTypeHeli:
			resourceID = isAnchorDirectionDown ? IDR_WAVE_PLANE_ANCHOR_DOWN : IDR_WAVE_PLANE_ANCHOR_UP;
			volume = g_settings.audio.volumeAnchorSeaPlane;
			break;

		default:
		case VehicleTypeUnknown:
			return;
		}

		// play audio
		AudioPlayer::play(resourceID, volume);
	}
}



void update()
{
	if (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
		return;

	// Update Notifications
	Notifications::update();

	// Update controls
	g_enterSubControlsWatcher.update();

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		return;

	bool isPlayerInAnySub = PED::IS_PED_IN_ANY_SUB(playerPed);

	// check if player is on any boat or sub
	if (PED::IS_PED_IN_ANY_BOAT(playerPed) || isPlayerInAnySub || PED::IS_PED_IN_ANY_PLANE(playerPed) || PED::IS_PED_IN_ANY_HELI(playerPed))
	{
		handlePlayerOnBoat(playerPed, isPlayerInAnySub);
	}
	else
	{
		handlePlayerOutsideBoat(playerPed);
	}

	// handle entity distance removal	
	handleEntityDistanceRemoval(playerPed);	

	
	#ifdef DEBUG_DRAW_OVERLAY
	// draw debug overlay
	DebugFrameOverlay::drawDebugOverlay(playerPed, g_vehicleManager.getLast(), g_enterSubControlsWatcher);
	#endif

	#ifdef DEBUG_VEHICLE_SPAWN

	if (Keyboard::IsKeyJustDown(0x7B)) //F12
	{
		DebugVehicleSpawner::spawnVehicleAtPedLocation(playerPed, VehicleEntity::eVehicleHash::VehicleHashSeashark1);
	}
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
	// load settings from ini file
	Settings::loadFromFile(g_settings, TEXT("AdvancedAnchor.ini"));

	//clear old vehicles, if it is called agin after loading a savegame
	g_vehicleManager.clear();

	while (true)
	{
		update();
		WAIT(0);
	}
}


void cleanupScript()
{
	// Cleanup audio resources
	AudioPlayer::dispose();

	#ifdef DEBUG_CONSOLE
	// dispose debug console
	DebugConsole::dispose();
	#endif	
}
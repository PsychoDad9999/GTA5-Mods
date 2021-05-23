// ----------------------------------------------------------------------------

#include "debugFrameOverlay.h"

#include <cstdio>

#include "inc/natives.h"
#include "framework/hud/text.h"
#include "framework/input/keyboard.h"
#include "framework/world/world.h"

// ----------------------------------------------------------------------------
/// <summery>Draw some ingame debug information</summery>
/// <param name="playerPed">player ped</param>
/// <param name="lastVehicle">last anchored vehicle</param>
// ----------------------------------------------------------------------------
void DebugFrameOverlay::drawDebugOverlay(Ped playerPed, AnchoredVehicle* const lastVehicle, ControlsWatcher& controlswatcher)
{
	Vector3 playerPos = World::getPosition(playerPed);

	char playerPosText[256];
	sprintf_s(playerPosText, "Player Pos: %07.1f/%07.1f/%07.1f", playerPos.x, playerPos.y, playerPos.z);

	Text::displayTextForCurrentFrame(playerPosText, 0.175, 0.0, true);


	if (lastVehicle != nullptr)
	{
		ScrHandle handle;

		char* script = ENTITY::GET_ENTITY_SCRIPT(lastVehicle->vehicle, &handle);

		if (script != NULL)
		{
			Text::displayTextForCurrentFrame(script, 0.5, 0.1, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("No Script", 0.5, 0.1, false);
		}


		float downForce = VEHICLE::_GET_VEHICLE_MODEL_DOWN_FORCE(ENTITY::GET_ENTITY_MODEL(lastVehicle->vehicle));

		char downForceText[256];
		sprintf_s(downForceText, "Down Force: %.1f", downForce);
		Text::displayTextForCurrentFrame(downForceText, 0.5, 0.15, true);



		char position[256];
		sprintf_s(position, "Anchor Pos: %07.1f/%07.1f/%07.1f", lastVehicle->blipPosition.x, lastVehicle->blipPosition.y, lastVehicle->blipPosition.z);

		if (lastVehicle->isSecured)
			Text::displayTextForCurrentFrame(position, 0.175, 0.05, true);
		else
			Text::displayTextForCurrentFrame(position, 0.175, 0.05, false);		


		char text[256];
		sprintf_s(text, "Vehicle ID: %d", lastVehicle->vehicle);

		Text::displayTextForCurrentFrame(text, 0.1, 0.15, true);

		if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(lastVehicle->vehicle)))
		{
			if (VEHICLE::_IS_THIS_MODEL_A_JETSKI(ENTITY::GET_ENTITY_MODEL(lastVehicle->vehicle)))
			{
				Text::displayTextForCurrentFrame("Type: Jet Ski", 0.1, 0.2, true);
			}
			else
			{
				Text::displayTextForCurrentFrame("Type: Boat", 0.1, 0.2, true);
			}
		}
		else
		{
			Text::displayTextForCurrentFrame("Type: ???", 0.1, 0.2, false);
		}


		if (VEHICLE::_GET_BOAT_ANCHOR(lastVehicle->vehicle))
			Text::displayTextForCurrentFrame("Has Anchor: Yes", 0.1, 0.25, true);
		else
			Text::displayTextForCurrentFrame("Has Anchor: No", 0.1, 0.25, false);

		if (lastVehicle->isAnchored)
			Text::displayTextForCurrentFrame("Anchored: Yes", 0.1, 0.3, true);
		else
			Text::displayTextForCurrentFrame("Anchored: No", 0.1, 0.3, false);

		if (lastVehicle->isSecured)
			Text::displayTextForCurrentFrame("Secured: Yes", 0.1, 0.35, true);
		else
			Text::displayTextForCurrentFrame("Secured: No", 0.1, 0.35, false);

		if (lastVehicle->isReentered)
			Text::displayTextForCurrentFrame("Re-Entered: Yes", 0.1, 0.4, true);
		else
			Text::displayTextForCurrentFrame("Re-Entered: No", 0.1, 0.4, false);

		Blip blip = UI::GET_BLIP_FROM_ENTITY(lastVehicle->vehicle);

		if (UI::DOES_BLIP_EXIST(blip))
			Text::displayTextForCurrentFrame("Blip: Yes", 0.1, 0.45, true);
		else
			Text::displayTextForCurrentFrame("Blip: No", 0.1, 0.45, false);

		if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(lastVehicle->vehicle))
			Text::displayTextForCurrentFrame("Persistent: Yes", 0.1, 0.5, true);
		else
			Text::displayTextForCurrentFrame("Persistent: No", 0.1, 0.5, false);


		char distance[256];
		sprintf_s(distance, "Blip Distance: %.1f m", World::getDistance(playerPos, lastVehicle->blipPosition, true));

		if (UI::DOES_BLIP_EXIST(blip))
			Text::displayTextForCurrentFrame(distance, 0.1, 0.55, true);
		else
			Text::displayTextForCurrentFrame("Blip Distance: N/A", 0.1, 0.55, false);
	}
	else
	{
		Text::displayTextForCurrentFrame("Vehicle ID: N/A", 0.1, 0.15, false);
	}


	Vehicle currentVehicle = 0;

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		// Player is not in any vehicle. Get last vehicle
		currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, FALSE);
		Text::displayTextForCurrentFrame("Is current Vehicle", 0.9, 0.10, true);
	}
	else
	{
		currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, TRUE);
		Text::displayTextForCurrentFrame("Is last Vehicle", 0.9, 0.10, false);
	}

	if (currentVehicle != 0)
	{
		char text[256];
		sprintf_s(text, "Vehicle ID: %d", currentVehicle);

		Text::displayTextForCurrentFrame(text, 0.9, 0.15, true);

		if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
		{
			if (VEHICLE::_IS_THIS_MODEL_A_JETSKI(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Jet Ski", 0.9, 0.2, true);
			else
				Text::displayTextForCurrentFrame("Type: Boat", 0.9, 0.2, true);
		}
		else
		{
			if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Plane", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Helicopter", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Car", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_TRAIN(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Train", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Bike", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_BICYCLE(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Bicycle", 0.9, 0.2, true);
			else if (VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(currentVehicle)))
				Text::displayTextForCurrentFrame("Type: Quad", 0.9, 0.2, true);
			else
				Text::displayTextForCurrentFrame("Type: ???", 0.9, 0.2, false);
		}

		if (VEHICLE::_GET_BOAT_ANCHOR(currentVehicle))
			Text::displayTextForCurrentFrame("Has Anchor: Yes", 0.9, 0.25, true);
		else
			Text::displayTextForCurrentFrame("Has Anchor: No", 0.9, 0.25, false);


		if (VEHICLE::IS_BIG_VEHICLE(currentVehicle))
			Text::displayTextForCurrentFrame("Is Big: Yes", 0.9, 0.3, true);
		else
			Text::displayTextForCurrentFrame("Is Big: No", 0.9, 0.3, false);

		if (ENTITY::IS_ENTITY_IN_WATER(currentVehicle))
			Text::displayTextForCurrentFrame("In Water: Yes", 0.9, 0.35, true);
		else
			Text::displayTextForCurrentFrame("In Water: No", 0.9, 0.35, false);


		float level = ENTITY::GET_ENTITY_SUBMERGED_LEVEL(currentVehicle);

		char textSub[256];
		sprintf_s(textSub, "Submerged Level: %.1f", level);

		Text::displayTextForCurrentFrame(textSub, 0.9, 0.4, true);


		if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(currentVehicle))
			Text::displayTextForCurrentFrame("Persistent: Yes", 0.9, 0.45, true);
		else
			Text::displayTextForCurrentFrame("Persistent No", 0.9, 0.45, false);


		if (Keyboard::IsKeyDown(0x57)) // W KEy
		{
			Text::displayTextForCurrentFrame("W-Key IsDown: Yes", 0.9, 0.5, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("W-Key IsDown: No", 0.9, 0.5, false);
		}

		if (Keyboard::IsKeyJustUp(0x57, true))
		{
			Text::displayTextForCurrentFrame("W-Key Just Up: Yes", 0.9, 0.55, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("W-Key Just Up: No", 0.9, 0.55, false);
		}

		if (Keyboard::IsKeyJustDown(0x57))
		{
			Text::displayTextForCurrentFrame("W-Key Just Down: Yes", 0.9, 0.6, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("W-Key Just Down: No", 0.9, 0.6, false);
		}


		if (controlswatcher.isHoldDown())
		{
			Text::displayTextForCurrentFrame("Enter Control Holde Down: Yes", 0.9, 0.65, true);
		}
		else
		{
			Text::displayTextForCurrentFrame("Enter Control Holde Down: No", 0.9, 0.65, false);
		}

	}
	else
	{
		Text::displayTextForCurrentFrame("Vehicle ID: N/A", 0.9, 0.15, false);
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"
#include "vehicle/anchoredVehicle.h"
#include "input/controls.h"

// ----------------------------------------------------------------------------

class DebugFrameOverlay
{
public:
	static void drawDebugOverlay(Ped playerPed, AnchoredVehicle* const lastVehicle, ControlsWatcher& controlswatcher);

private:
	DebugFrameOverlay() {}
};

// ----------------------------------------------------------------------------
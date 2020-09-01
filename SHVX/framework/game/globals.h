// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

#include "framework/game/gameVersion.h"

// ----------------------------------------------------------------------------


enum eGlobals : int
{
	// Used in Script UnderwaterPickups. Must be set before the script is loaded
	UNDERWATER_PICKUP_RESPAWN_TIME,
};

class Globals
{
public:
	static UINT64* getGlobal(eGlobals global, eExtGameVersion gameVersion);

private:
	Globals();

};


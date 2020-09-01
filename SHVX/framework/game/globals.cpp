// ----------------------------------------------------------------------------

#include "globals.h"

#include "../SHV/inc/main.h"

// ----------------------------------------------------------------------------


UINT64* Globals::getGlobal(eGlobals global, eExtGameVersion gameVersion)
{
	switch (global)
	{
	case eGlobals::UNDERWATER_PICKUP_RESPAWN_TIME:
		switch (gameVersion)
		{
		case eExtGameVersion::BUILD_1_0_1737_0_STEAM:
		case eExtGameVersion::BUILD_1_0_1737_0_NOSTEAM:
		case eExtGameVersion::BUILD_1_0_1737_6_STEAM:
		case eExtGameVersion::BUILD_1_0_1737_6_NOSTEAM:
			return getGlobalPtr(2460031);		

		case eExtGameVersion::BUILD_1_0_1868_0_STEAM:
		case eExtGameVersion::BUILD_1_0_1868_0_NOSTEAM:
		case eExtGameVersion::BUILD_1_0_1868_1_STEAM:
		case eExtGameVersion::BUILD_1_0_1868_1_NOSTEAM:
			return getGlobalPtr(2460417);

		case eExtGameVersion::BUILD_1_0_2060_0_STEAM:
		case eExtGameVersion::BUILD_1_0_2060_0_EPIC:
		case eExtGameVersion::BUILD_1_0_2060_0_ROCKSTAR:
			return getGlobalPtr(2462098);
		}
		break;	
	
	default:
		return nullptr;
	}

	return nullptr;
}
// ----------------------------------------------------------------------------

#include "globals.h"

#include <inc/main.h>

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
		case eExtGameVersion::BUILD_1_0_1868_4_EPIC:
			return getGlobalPtr(2460417);

		case eExtGameVersion::BUILD_1_0_2060_0_STEAM:
		case eExtGameVersion::BUILD_1_0_2060_0_NOSTEAM:		
		case eExtGameVersion::BUILD_1_0_2060_1_STEAM:
		case eExtGameVersion::BUILD_1_0_2060_1_NOSTEAM:		
			return getGlobalPtr(2462098);

		case eExtGameVersion::BUILD_1_0_2189_0_STEAM:
		case eExtGameVersion::BUILD_1_0_2189_0_NOSTEAM:
			return getGlobalPtr(2463669);

		case eExtGameVersion::BUILD_1_0_2245_0_STEAM:
		case eExtGameVersion::BUILD_1_0_2245_0_NOSTEAM:
			return getGlobalPtr(2463897);

		case eExtGameVersion::BUILD_1_0_2372_0_STEAM:
		case eExtGameVersion::BUILD_1_0_2372_0_NOSTEAM:
			return getGlobalPtr(2464920);
		}
		break;	
	
	default:
		return nullptr;
	}

	return nullptr;
}
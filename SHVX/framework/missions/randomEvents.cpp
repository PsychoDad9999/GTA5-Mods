// ----------------------------------------------------------------------------

#include "randomEvents.h"

#include "inc/natives.h"

#include "framework/system/hashKey.h"

// ----------------------------------------------------------------------------

char* RandomEvents::getName(eRandomEvent randomEvent)
{
	switch (randomEvent)
	{
	case eRandomEvent::RE_NONE:
		return "RE_NONE";
		
	case eRandomEvent::RE_ACCIDENT:
		return "RE_ACCIDENT";

	case eRandomEvent::RE_ATMROBBERY:
		return "RE_ATMROBBERY";

	case eRandomEvent::RE_BUSTOUR:
		return "RE_BUSTOUR";

	case eRandomEvent::RE_DOMESTIC:
		return "RE_DOMESTIC";

	case eRandomEvent::RE_GETAWAYDRIVER:
		return "RE_GETAWAYDRIVER";

	case eRandomEvent::RE_SHOPROBBERY:
		return "RE_SHOPROBBERY";

	case eRandomEvent::RE_SNATCHED:
		return "RE_SNATCHED";

	case eRandomEvent::RE_LURED:
		return "RE_LURED";

	case eRandomEvent::RE_BIKETHIEFSTAMP:
		return "RE_BIKETHIEFSTAMP";
		
	case eRandomEvent::RE_SECURITYVAN:
		return "RE_SECURITYVAN";		

	case eRandomEvent::RE_PAPARAZZI:
		return "RE_PAPARAZZI";		

	case eRandomEvent::RE_CHASETHIEVES:
		return "RE_CHASETHIEVES";		

	case eRandomEvent::RE_DEALGONEWRONG:
		return "RE_DEALGONEWRONG";		

	case eRandomEvent::RE_HITCHLIFT:
		return "RE_HITCHLIFT";		

	case eRandomEvent::RE_STAG:
		return "RE_STAG";		

	case eRandomEvent::RE_ARREST:
		return "RE_ARREST";		

	case eRandomEvent::RE_CRASHRESCUE:
		return "RE_CRASHRESCUE";		

	case eRandomEvent::RE_CARTHEFT:
		return "RE_CARTHEFT";		

	case eRandomEvent::RE_CULTSHOOTOUT:
		return "RE_CULTSHOOTOUT";		

	case eRandomEvent::RE_GANGFIGHT:
		return "RE_GANGFIGHT";		

	case eRandomEvent::RE_GANGINTIMIDATION:
		return "RE_GANGINTIMIDATION";	

	case eRandomEvent::RE_PRISONVANBREAK:
		return "RE_PRISONVANBREAK";		

	case eRandomEvent::RE_PRISONERLIFT:
		return "RE_PRISONERLIFT";		

	case eRandomEvent::RE_ABANDONEDCAR:
		return "RE_ABANDONEDCAR";		

	case eRandomEvent::RE_BURIAL:
		return "RE_BURIAL";		

	case eRandomEvent::RE_MUGGING:
		return "RE_MUGGING";		

	case eRandomEvent::RE_BIKETHIEF:
		return "RE_BIKETHIEF";		

	case eRandomEvent::RE_DRUNKDRIVER:
		return "RE_DRUNKDRIVER";		

	case eRandomEvent::RE_HOMELANDSECURITY:
		return "RE_HOMELANDSECURITY";		

	case eRandomEvent::RE_BORDERPATROL:
		return "RE_BORDERPATROL";		

	case eRandomEvent::RE_SIMEONYETARIAN:
		return "RE_SIMEONYETARIAN";		

	case eRandomEvent::RE_DUEL:
		return "RE_DUEL";		

	case eRandomEvent::RE_SEAPLANE:
		return "RE_SEAPLANE";		

	case eRandomEvent::RE_MONKEYPHOTO:
		return "RE_MONKEYPHOTO";		

	default:
		return "UNKNOWN";
	}	
}


int RandomEvents::getCoolDownTimeInHours(eRandomEvent randomEvent)
{
	switch (randomEvent)
	{
	case eRandomEvent::RE_ACCIDENT:
		return 30;

	case eRandomEvent::RE_ATMROBBERY:
		return 200;

	case eRandomEvent::RE_BUSTOUR:
		return 0;

	case eRandomEvent::RE_DOMESTIC:
		return 60;

	case eRandomEvent::RE_GETAWAYDRIVER:
		return 60;

	case eRandomEvent::RE_SHOPROBBERY:
		return 30;

	case eRandomEvent::RE_SNATCHED:
		return 40;

	case eRandomEvent::RE_LURED:
		return 30;

	case eRandomEvent::RE_BIKETHIEFSTAMP:
		return 30;

	case eRandomEvent::RE_SECURITYVAN:
		return 60;

	case eRandomEvent::RE_PAPARAZZI:
		return 30;

	case eRandomEvent::RE_CHASETHIEVES:
		return 30;

	case eRandomEvent::RE_DEALGONEWRONG:
		return 120;

	case eRandomEvent::RE_HITCHLIFT:
		return 35;

	case eRandomEvent::RE_STAG:
		return 40;

	case eRandomEvent::RE_ARREST:
		return 30;

	case eRandomEvent::RE_CRASHRESCUE:
		return 30;

	case eRandomEvent::RE_CARTHEFT:
		return 30;

	case eRandomEvent::RE_CULTSHOOTOUT:
		return 30;

	case eRandomEvent::RE_GANGFIGHT:
		return 30;

	case eRandomEvent::RE_GANGINTIMIDATION:
		return 30;

	case eRandomEvent::RE_PRISONVANBREAK:
		return 30;

	case eRandomEvent::RE_PRISONERLIFT:
		return 30;

	case eRandomEvent::RE_ABANDONEDCAR:
		return 30;

	case eRandomEvent::RE_BURIAL:
		return 30;

	case eRandomEvent::RE_MUGGING:
		return 40;

	case eRandomEvent::RE_BIKETHIEF:
		return 30;

	case eRandomEvent::RE_DRUNKDRIVER:
		return 60;

	case eRandomEvent::RE_HOMELANDSECURITY:
		return 30;

	case eRandomEvent::RE_BORDERPATROL:
		return 30;

	case eRandomEvent::RE_SIMEONYETARIAN:
		return 60;

	case eRandomEvent::RE_DUEL:
	case eRandomEvent::RE_SEAPLANE:
	case eRandomEvent::RE_MONKEYPHOTO:
	default:
		return 0;
	}
}


int RandomEvents::getVariations(eRandomEvent randomEvent)
{
	switch (randomEvent)
	{
	case eRandomEvent::RE_ATMROBBERY:
		return 5;		

	case eRandomEvent::RE_SHOPROBBERY:
		return 2;

	case eRandomEvent::RE_SECURITYVAN:
		return 10;		

	case eRandomEvent::RE_CHASETHIEVES:
		return 4;		

	case eRandomEvent::RE_HITCHLIFT:
		return 4;

	case eRandomEvent::RE_ARREST:
		return 2;

	case eRandomEvent::RE_CARTHEFT:
		return 2;		

	case eRandomEvent::RE_PRISONERLIFT:
		return 2;		

	case eRandomEvent::RE_ABANDONEDCAR:
		return 2;

	case eRandomEvent::RE_MUGGING:
		return 3;		

	case eRandomEvent::RE_BIKETHIEF:
		return 2;		

	case eRandomEvent::RE_DRUNKDRIVER:
		return 2;		

	case eRandomEvent::RE_BORDERPATROL:
		return 3;
		
	default:
		return 1;
	}	
}




eRandomEvent RandomEvents::GetRandomEventByScriptName(char* scriptName)
{	
	Hash hash = GAMEPLAY::GET_HASH_KEY(scriptName);

	if (hash == HashKey::GetHashKey("re_abandonedcar"))
	{
		return eRandomEvent::RE_ABANDONEDCAR;
	}
	else if (hash == HashKey::GetHashKey("re_accident"))
	{
		return eRandomEvent::RE_ACCIDENT;
	}
	else if (hash == HashKey::GetHashKey("re_arrests"))
	{
		return eRandomEvent::RE_ARREST;
	}
	else if (hash == HashKey::GetHashKey("re_atmrobbery"))
	{
		return eRandomEvent::RE_ATMROBBERY;
	}
	else if (hash == HashKey::GetHashKey("re_bikethief"))
	{
		return eRandomEvent::RE_BIKETHIEF;
	}
	else if (hash == HashKey::GetHashKey("re_border"))
	{
		return eRandomEvent::RE_BORDERPATROL;
	}
	else if (hash == HashKey::GetHashKey("re_burials"))
	{
		return eRandomEvent::RE_BUSTOUR;
	}
	else if (hash == HashKey::GetHashKey("re_cartheft"))
	{
		return eRandomEvent::RE_CARTHEFT;
	}
	else if (hash == HashKey::GetHashKey("re_chasethieves"))
	{
		return eRandomEvent::RE_CHASETHIEVES;
	}
	else if (hash == HashKey::GetHashKey("re_crashrescue"))
	{
		return eRandomEvent::RE_CRASHRESCUE;
	}
	else if (hash == HashKey::GetHashKey("re_cultshootout"))
	{
		return eRandomEvent::RE_CULTSHOOTOUT;
	}
	else if (hash == HashKey::GetHashKey("re_dealgonewrong"))
	{
		return eRandomEvent::RE_DEALGONEWRONG;
	}
	else if (hash == HashKey::GetHashKey("re_domestic"))
	{
		return eRandomEvent::RE_DOMESTIC;
	}
	else if (hash == HashKey::GetHashKey("re_drunkdriver"))
	{
		return eRandomEvent::RE_DRUNKDRIVER;
	}
	else if (hash == HashKey::GetHashKey("re_gang_intimidation"))
	{
		return eRandomEvent::RE_GANGINTIMIDATION;
	}
	else if (hash == HashKey::GetHashKey("re_gangfight"))
	{
		return eRandomEvent::RE_GANGFIGHT;
	}
	else if (hash == HashKey::GetHashKey("re_getaway_driver"))
	{
		return eRandomEvent::RE_GETAWAYDRIVER;
	}
	else if (hash == HashKey::GetHashKey("re_hitch_lift"))
	{
		return eRandomEvent::RE_HITCHLIFT;
	}
	else if (hash == HashKey::GetHashKey("re_homeland_security"))
	{
		return eRandomEvent::RE_HOMELANDSECURITY;
	}
	else if (hash == HashKey::GetHashKey("re_lured"))
	{
		return eRandomEvent::RE_LURED;
	}
	else if (hash == HashKey::GetHashKey("re_muggings"))
	{
		return eRandomEvent::RE_MUGGING;
	}
	else if (hash == HashKey::GetHashKey("re_paparazzi"))
	{
		return eRandomEvent::RE_PAPARAZZI;
	}
	else if (hash == HashKey::GetHashKey("re_prisonerlift"))
	{
		return eRandomEvent::RE_PRISONERLIFT;
	}
	else if (hash == HashKey::GetHashKey("re_prisonvanbreak"))
	{
		return eRandomEvent::RE_PRISONVANBREAK;
	}
	else if (hash == HashKey::GetHashKey("re_securityvan"))
	{
		return eRandomEvent::RE_SECURITYVAN;
	}
	else if (hash == HashKey::GetHashKey("re_shoprobbery"))
	{
		return eRandomEvent::RE_SHOPROBBERY;
	}
	else if (hash == HashKey::GetHashKey("re_snatched"))
	{
		return eRandomEvent::RE_SNATCHED;
	}
	else if (hash == HashKey::GetHashKey("re_stag_do"))
	{
		return eRandomEvent::RE_STAG;
	}
	else if (hash == HashKey::GetHashKey("re_yetarian"))
	{
		return eRandomEvent::RE_SIMEONYETARIAN;
	}
	else if (hash == HashKey::GetHashKey("re_duel"))
	{
		return eRandomEvent::RE_DUEL;
	}
	else if (hash == HashKey::GetHashKey("re_seaplane"))
	{
		return eRandomEvent::RE_SEAPLANE;
	}
	else if (hash == HashKey::GetHashKey("re_monkey"))
	{
		return eRandomEvent::RE_MONKEYPHOTO;
	}
	else
	{
		return eRandomEvent::RE_NONE;
	}
}
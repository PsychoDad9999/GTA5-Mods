// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

enum eRandomEvent
{
	RE_NONE = -1,
	RE_ACCIDENT = 0,
	RE_ATMROBBERY = 1,
	RE_BUSTOUR = 2,
	RE_DOMESTIC = 3,
	RE_GETAWAYDRIVER = 4,
	RE_SHOPROBBERY = 5,
	RE_SNATCHED = 6,
	RE_LURED = 7,
	RE_BIKETHIEFSTAMP = 8,
	RE_SECURITYVAN = 9,
	RE_PAPARAZZI = 10,
	RE_CHASETHIEVES = 11,
	RE_DEALGONEWRONG = 12,
	RE_HITCHLIFT = 13,
	RE_STAG = 14,
	RE_ARREST = 15,
	RE_CRASHRESCUE = 16,
	RE_CARTHEFT = 17,
	RE_CULTSHOOTOUT = 18,
	RE_GANGFIGHT = 19,
	RE_GANGINTIMIDATION = 20,
	RE_PRISONVANBREAK = 21,
	RE_PRISONERLIFT = 22,
	RE_ABANDONEDCAR = 23,
	RE_BURIAL = 24,
	RE_MUGGING = 25,
	RE_BIKETHIEF = 26,
	RE_DRUNKDRIVER = 27,
	RE_HOMELANDSECURITY = 28,
	RE_BORDERPATROL = 29,
	RE_SIMEONYETARIAN = 30,
	RE_DUEL = 31,
	RE_SEAPLANE = 32,
	RE_MONKEYPHOTO = 33,
};


class RandomEvents
{
public:
	static char* getName(eRandomEvent randomEvent);
	static int getVariations(eRandomEvent randomEvent);
	static int getCoolDownTimeInHours(eRandomEvent randomEvent);
	static eRandomEvent GetRandomEventByScriptName(char* scriptName);

private:
	RandomEvents();
};

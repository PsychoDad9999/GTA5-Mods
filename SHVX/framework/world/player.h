// ----------------------------------------------------------------------------

#pragma once

#include "../SHV/inc/types.h"

// ----------------------------------------------------------------------------

#define MODELHASH_MICHAEL	0xD7114C9
#define MODELHASH_FRANKLIN	0x9B22DBAF
#define MODELHASH_TREVOR	0x9B810FA2

enum ePlayerIdentifier : int
{	
	UNKNOWN = -1,
	MICHAEL = 0,
	FRANKLIN = 1,
	TREVOR = 2	
};


class PlayerIdentifier
{
public:
	static ePlayerIdentifier getPlayerIdentifierFromModelHash(Hash modelHash)
	{
		switch (modelHash)
		{
		case MODELHASH_MICHAEL:
			return ePlayerIdentifier::MICHAEL;

		case MODELHASH_FRANKLIN:
			return ePlayerIdentifier::FRANKLIN;

		case MODELHASH_TREVOR:
			return ePlayerIdentifier::TREVOR;

		default:
			return ePlayerIdentifier::UNKNOWN;
		}
	}

private:
	PlayerIdentifier() {};
};


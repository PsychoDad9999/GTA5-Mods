// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

enum eMessageType : int
{	
	MISSION_PASSED_MESSAGE,
	MISSION_FAILED_MESSAGE,
};


class Scaleform
{
public:
	Scaleform(eMessageType messageType, char* message, ULONGLONG duration);
	~Scaleform();

	bool update();

private:
	void render();

	bool isLoaded();
	bool isValidHandle();


private:
	int m_scaleformHandle = 0;

	ULONGLONG m_duration = 0;
	ULONGLONG m_startTime = 0;
};


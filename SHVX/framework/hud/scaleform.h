// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include "framework/world/player.h"

// ----------------------------------------------------------------------------

class Scaleform
{
public:
	enum eState
	{
		StartAudio,
		StartPostFX,
		StartRollup,

		ShowButtons,
		WaitForInput,

		WaitForInput2,

		StartRollOut,		
		EndRollOut,
		Completed,
	};


public:
	Scaleform(const char* message, ePlayerIdentifier playerIdentifier, ULONGLONG duration);
	~Scaleform();

	bool update();

private:
	void render();

	bool isLoaded();
	bool isValidHandle();

	bool isLoadedScaleformButton();
	bool isValidHandleScaleformButton();
	

	void playSuccessAudio(ePlayerIdentifier playerIdentifier);
	void playContinueAudio();
	
	char* getPostFXSuccessName(ePlayerIdentifier playerIdentifier);
	char* getAudioName(ePlayerIdentifier playerIdentifier, bool isBig);

	bool addRollupTransition();   // Roll up
	bool addRolloutTransition(); // Roll out of screen
	bool createButtons(bool showExpandButton);

private:
	eState m_currentState = eState::StartAudio;

	bool m_drawButtons = true;

private:
	int m_scaleformHandle = 0;
	int m_scaleformButtonHandle = 0;

	bool m_rollupTransitionCreated = false;	

	ULONGLONG m_duration = 0;
	ULONGLONG m_startTime = 0;
	ePlayerIdentifier m_playerIdentifier = ePlayerIdentifier::UNKNOWN;
};


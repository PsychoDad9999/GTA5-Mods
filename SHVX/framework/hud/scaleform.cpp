// ----------------------------------------------------------------------------

#include "Scaleform.h"

#include <inc/natives.h>

// ----------------------------------------------------------------------------

Scaleform::Scaleform(const char* message, ePlayerIdentifier playerIdentifier, ULONGLONG duration)
{
	m_playerIdentifier = playerIdentifier;
	m_duration = duration;
	m_scaleformHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_BIG_MESSAGE_FREEMODE");
	m_scaleformButtonHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE_INSTANCE("INSTRUCTIONAL_BUTTONS");
	
	if (isValidHandle())
	{
		while (!isLoaded())
		{
			WAIT(0);
		}

		if (isValidHandleScaleformButton())
		{
			while (!isLoadedScaleformButton())
			{
				WAIT(0);
			}
		}

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformHandle, "SHOW_MISSION_PASSED_MESSAGE");

		GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("M_FB4P3_P");
		GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();						
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(const_cast<char*>(message));
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(100);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(TRUE);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(TRUE);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD(); 
	}

	m_startTime = -1;
}

Scaleform::~Scaleform()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&m_scaleformHandle);
	}

	if (isValidHandleScaleformButton() && isLoadedScaleformButton())
	{
		GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&m_scaleformButtonHandle);
	}
}


bool Scaleform::isValidHandle()
{
	return m_scaleformHandle != 0;
}

bool Scaleform::isLoaded()
{
	return GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_scaleformHandle);
}


bool Scaleform::isValidHandleScaleformButton()
{
	return m_scaleformButtonHandle != 0;
}

bool Scaleform::isLoadedScaleformButton()
{
	return GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_scaleformButtonHandle);
}





bool Scaleform::update()
{
	if (m_currentState == eState::StartAudio)
	{
		if (isValidHandle() && isLoaded())
		{
			m_startTime = 0;
			playSuccessAudio(m_playerIdentifier);
			m_currentState = eState::StartPostFX;
		}
	}

	if (m_currentState == eState::StartPostFX)
	{
		if (AUDIO::_0x6F259F82D873B8B8())  // AUDIO::IS_MISSION_COMPLETE_READY_FOR_UI()
		{
			AUDIO::UNREGISTER_SCRIPT_WITH_AUDIO();
			GRAPHICS::ANIMPOSTFX_PLAY(getPostFXSuccessName(m_playerIdentifier), 1000, FALSE);
			m_startTime = GetTickCount64();
			m_currentState = eState::StartRollup;
		}
	}

	if (m_currentState == eState::StartRollup)
	{
		if (GetTickCount64() - m_startTime > 600)  // start rollup transition after 600ms
		{					
			if (addRollupTransition())
			{
				m_startTime = GetTickCount64();
				m_currentState = eState::ShowButtons;
			}
		}
	}

	if (m_currentState == eState::ShowButtons)
	{
		if (GetTickCount64() - m_startTime > 1000)
		{
			if (createButtons(true))
			{
				m_startTime = GetTickCount64();
				m_currentState = eState::WaitForInput;
			}
		}
	}

	if (m_currentState == eState::WaitForInput)
	{
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 215))  // Continue
		{
			playContinueAudio();
			m_startTime = GetTickCount64();
			m_currentState = eState::StartRollOut;
		}
		else if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 216))  // Expand
		{
			if (createButtons(false))
			{
				m_startTime = GetTickCount64();
				m_currentState = eState::WaitForInput2;
			}
		}
	}

	if (m_currentState == eState::WaitForInput2)
	{
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 215))  // Continue
		{
			playContinueAudio();
			m_startTime = GetTickCount64();
			m_currentState = eState::StartRollOut;
		}
	}


	if (m_currentState == eState::StartRollOut)
	{
		m_drawButtons = false;

		if (addRolloutTransition())
		{
			m_startTime = GetTickCount64();
			m_currentState = eState::EndRollOut;
		}
	}	

	if (m_currentState == eState::EndRollOut)
	{
		if (GetTickCount64() - m_startTime > 600)
		{
			m_currentState = eState::Completed;
		}
	}
	

	if (m_currentState >= eState::StartRollup && m_currentState < eState::Completed)
	{				
		render();		
	}

	return m_currentState != eState::Completed;
}


bool Scaleform::addRollupTransition()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformHandle, "TRANSITION_UP");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_FLOAT(0.15f);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(TRUE);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		return true;
	}

	return false;
}


bool Scaleform::addRolloutTransition()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformHandle, "TRANSITION_OUT");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		return true;
	}

	return false;
}


bool Scaleform::createButtons(bool showExpandButton)
{
	if (isValidHandleScaleformButton() && isLoadedScaleformButton())
	{
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformButtonHandle, "CLEAR_ALL");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		if (GAMEPLAY::IS_PC_VERSION())
		{
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformButtonHandle, "TOGGLE_MOUSE_BUTTONS");
			GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(true);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		}

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformButtonHandle, "SET_DATA_SLOT");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(CONTROLS::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, 215, 1));

		GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("ES_HELP");  // Use localized string of "Continue"
		GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();

		if (GAMEPLAY::IS_PC_VERSION())
		{
			GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(true);
			GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(215);
		}
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		if (showExpandButton)
		{
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformButtonHandle, "SET_DATA_SLOT");
			GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(1);
			GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(CONTROLS::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, 216, 1));

			GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("ES_XPAND");  // Use localized string of "Expand"
			GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();

			if (GAMEPLAY::IS_PC_VERSION())
			{
				GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(true);
				GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(216);
			}
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		}

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformButtonHandle, "DRAW_INSTRUCTIONAL_BUTTONS");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		return true;
	}

	return false;
}



void Scaleform::render()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_scaleformHandle, 255, 255, 255, 255, 0);
	}

	if (m_drawButtons)
	{
		if (isValidHandleScaleformButton() && isLoadedScaleformButton())
		{
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_scaleformButtonHandle, 255, 255, 255, 255, 0);			
		}
	}
}


void Scaleform::playSuccessAudio(ePlayerIdentifier playerIdentifier)
{
	AUDIO::REGISTER_SCRIPT_WITH_AUDIO(FALSE);
	AUDIO::PLAY_MISSION_COMPLETE_AUDIO(getAudioName(playerIdentifier, false));	
}


void Scaleform::playContinueAudio()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "CONTINUE", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
}


char* Scaleform::getPostFXSuccessName(ePlayerIdentifier playerIdentifier)
{
	switch (playerIdentifier)
	{	
	case MICHAEL:
		return "SuccessMichael";

	case TREVOR:
		return "SuccessTrevor";

	case FRANKLIN:
	case UNKNOWN:
	default:	
		return "SuccessFranklin";			
	}
}

char* Scaleform::getAudioName(ePlayerIdentifier playerIdentifier, bool isBig)
{
	switch (playerIdentifier)
	{
	case MICHAEL:
		return isBig ? "MICHAEL_BIG_01" : "MICHAEL_SMALL_01";

	case TREVOR:
		return isBig ? "TREVOR_BIG_01" : "TREVOR_SMALL_01";

	case FRANKLIN:
	case UNKNOWN:
	default:
		return isBig ? "FRANKLIN_BIG_01" : "FRANKLIN_SMALL_01";
	}
}

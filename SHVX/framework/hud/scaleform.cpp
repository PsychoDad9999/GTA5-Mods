// ----------------------------------------------------------------------------

#include "Scaleform.h"

#include <inc/natives.h>

// ----------------------------------------------------------------------------

Scaleform::Scaleform(eMessageType messageType, char* message, ULONGLONG duration)
{
	m_duration = duration;
	m_scaleformHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_BIG_MESSAGE_FREEMODE");

	if (isValidHandle())
	{
		while (!isLoaded())
		{
			WAIT(0);
		}

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_scaleformHandle, "SHOW_MISSION_PASSED_MESSAGE");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(message);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING("");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(100);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(true);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_BOOL(true);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		// GRAPHICS::ANIMPOSTFX_PLAY("SuccessFranklin", 1000, FALSE);
	}

	m_startTime = GetTickCount64();
}

Scaleform::~Scaleform()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&m_scaleformHandle);
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


bool Scaleform::update()
{
	bool hasCompleted = GetTickCount64() - m_startTime > m_duration;

	if (!hasCompleted)
	{
		render();
	}

	return !hasCompleted;
}


void Scaleform::render()
{
	if (isValidHandle() && isLoaded())
	{
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_scaleformHandle, 255, 255, 255, 255, 0);
	}
}



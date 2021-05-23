// ----------------------------------------------------------------------------

#pragma once

#include "inc/types.h"

// ----------------------------------------------------------------------------

class ControlsWatcher
{
public:
	void update();	
	
	bool isHoldDown() 
	{ 
		return m_isHoldDown; 
	}

	void setIsHandled() 
	{ 
		m_isHandled = true; 
	}

public:
	ControlsWatcher(int inputGroup, int control, int holdDownDuration)
		: m_inputGroup(inputGroup), m_control(control), m_holdDownDuration(holdDownDuration)
	{
		m_isControlPressed = false;
		m_startTime = 0;
		m_isHoldDown = false;
		m_isHandled = false;
	}

private:
	int m_inputGroup;
	int m_control;
	int m_holdDownDuration;

private:
	int m_isControlPressed;
	ULONGLONG m_startTime;
	bool m_isHoldDown;
	bool m_isHandled;
};

// ----------------------------------------------------------------------------
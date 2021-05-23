// ----------------------------------------------------------------------------

#include "controls.h"

#include "inc/natives.h"

// ----------------------------------------------------------------------------

const int MAX_DURATION = 5000;

// ----------------------------------------------------------------------------

void ControlsWatcher::update()
{
	bool isControlPressed = CONTROLS::IS_CONTROL_PRESSED(m_inputGroup, m_control) == TRUE;

	if (isControlPressed)
	{
		if (m_isHandled)
		{
			m_startTime = 0;
			m_isHoldDown = false;
		}
		else
		{
			ULONGLONG now = GetTickCount64();

			if (!m_isControlPressed)
			{
				//save timestamp
				m_startTime = now;
				m_isHoldDown = false;
			}
			else
			{
				m_isHoldDown = (now > m_startTime + m_holdDownDuration && now < m_startTime + MAX_DURATION);
			}
		}
	}
	else
	{
		m_startTime = 0;
		m_isHoldDown = false;
		m_isHandled = false;
	}

	m_isControlPressed = isControlPressed;
}

// ----------------------------------------------------------------------------
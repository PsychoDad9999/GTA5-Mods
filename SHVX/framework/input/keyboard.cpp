// ----------------------------------------------------------------------------

#include "keyboard.h"

#include "../SHV/inc/main.h"

#include <stdio.h>

// ----------------------------------------------------------------------------

Keyboard::Key Keyboard::m_keystates[MAX_KEY_STATES];


void Keyboard::RegisterKeyboardHook()
{
	keyboardHandlerRegister(Keyboard::OnKeyboardMessage);
}

void Keyboard::UnregisterKeyboardHook()
{
	keyboardHandlerUnregister(Keyboard::OnKeyboardMessage);
}


// ----------------------------------------------------------------------------
/// <summery>Keyboard Handler Callback</summery>
/// <param name="key">key</param>
/// <param name="repeats">key</param>
/// <param name="scanCode">scan code</param>
/// <param name="isExtended"></param>
/// <param name="isWithAlt"></param>
/// <param name="wasDownBefore">True if key was down before, otherwise false. While pressing down a key, multiple callbacks can occur until this is actually set</param>
/// <param name="isUpNow">True if key is up, otherwise false</param>
/// <remarks>This callback must be registered with Native SDK function "keyboardHandlerRegister" on dll attach</remarks>
// ----------------------------------------------------------------------------
void Keyboard::OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < MAX_KEY_STATES)
	{
		m_keystates[key].time = GetTickCount();
		m_keystates[key].isWithAlt = isWithAlt;
		m_keystates[key].wasDownBefore = wasDownBefore;
		m_keystates[key].isUpNow = isUpNow;

		// Reset wasJustDown
		if (isUpNow)
			m_keystates[key].wasJustDown = false;
	}
}


// ----------------------------------------------------------------------------
/// <summery>Check if keycode is valid</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is valid, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsValidKey(DWORD key)
{
	return key > 0;
}


// ----------------------------------------------------------------------------
/// <summery>Check if key is pressed down</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is pressed down, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsKeyDown(DWORD key)
{
	if (key >= MAX_KEY_STATES)
		return false;

	return GetTickCount() < m_keystates[key].time + MAX_DOWN && !m_keystates[key].isUpNow;	
}


// ----------------------------------------------------------------------------
/// <summery>Check if key is pressed down long</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is pressed down long, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsKeyDownLong(DWORD key)
{
	if (key >= MAX_KEY_STATES)
		return false;

	DWORD now = GetTickCount();

	return now > m_keystates[key].time + LONG_DOWN && now < m_keystates[key].time + MAX_DOWN && !m_keystates[key].isUpNow;
}



// ----------------------------------------------------------------------------
/// <summery>Check if key is up</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is not pressed, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsKeyUp(DWORD key)
{
	return !IsKeyDown(key);
}


// ----------------------------------------------------------------------------
/// <summery>Check if key is pressed down</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is pressed down for the first time this function is called, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsKeyJustDown(DWORD key)
{
	if (key >= MAX_KEY_STATES)
		return false;

	bool isJustDown = GetTickCount() < m_keystates[key].time + NOW_PERIOD && !m_keystates[key].isUpNow && !m_keystates[key].wasDownBefore && !m_keystates[key].wasJustDown;

	// wasDownBefore doesn't work. save state of Just Down manually
	if (isJustDown)
		m_keystates[key].wasJustDown = true;

	return isJustDown;
}


// ----------------------------------------------------------------------------
/// <summery>Check if key is up</summery>
/// <param name="key">key code</param>
/// <returns>Retruns true if key is not pressed, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Keyboard::IsKeyJustUp(DWORD key, bool exclusive)
{
	if (key >= MAX_KEY_STATES)
		return false;

	bool isUp = GetTickCount() < m_keystates[key].time + NOW_PERIOD && m_keystates[key].isUpNow;

	if(isUp && exclusive)
		ResetKeyState(key);

	return isUp;
}


// ----------------------------------------------------------------------------
/// <summery>Clear Key state</summery>
/// <param name="key">key code</param>
// ----------------------------------------------------------------------------
void Keyboard::ResetKeyState(DWORD key)
{
	if (key < MAX_KEY_STATES)
		memset(&m_keystates[key], 0, sizeof(m_keystates[0]));
}

// ----------------------------------------------------------------------------
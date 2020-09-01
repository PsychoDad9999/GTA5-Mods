// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

class Keyboard
{
private:
	// This must be registered with Native SDK function "keyboardHandlerRegister"
	static void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

public:
	struct Key
	{
		DWORD time;
		BOOL isWithAlt;
		BOOL wasDownBefore;
		BOOL isUpNow;
		BOOL wasJustDown;
	};

public:
	static void RegisterKeyboardHook();
	static void UnregisterKeyboardHook();

public:
	static bool IsKeyDown(DWORD key);
	static bool IsKeyDownLong(DWORD key);
	static bool IsKeyUp(DWORD key);
	static bool IsKeyJustDown(DWORD key);
	static bool IsKeyJustUp(DWORD key, bool exclusive);

	static bool IsValidKey(DWORD key);
		
private:
	static void ResetKeyState(DWORD key);

private:
	static const int MAX_KEY_STATES = 255;
	static const int NOW_PERIOD = 100;
	static const int LONG_DOWN = 2000;
	static const int MAX_DOWN = 5000;

private:
	static Key m_keystates[MAX_KEY_STATES];

private:
	Keyboard() {}

};

// ----------------------------------------------------------------------------
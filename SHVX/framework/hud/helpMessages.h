// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <map>

// ----------------------------------------------------------------------------

class HelpMessages
{

public:
	static void showHelpMessage(const char* text);
	static void showHelpMessage(const char* text, int duration, bool beep);
	
	static void clearAll();

	static bool isHelpMessageBeingDisplayed();

private:
	HelpMessages();
};

// ----------------------------------------------------------------------------
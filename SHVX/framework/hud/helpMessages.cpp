// ----------------------------------------------------------------------------

#include "helpMessages.h"

#include "inc/natives.h"

// ----------------------------------------------------------------------------

void HelpMessages::showHelpMessage(const char* text)
{
	showHelpMessage(text, -1, true);
}

// ----------------------------------------------------------------------------

void HelpMessages::showHelpMessage(const char* text, int duration, bool beep)
{
	UI::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	UI::_ADD_TEXT_COMPONENT_SCALEFORM(const_cast<char*>(text));
	UI::END_TEXT_COMMAND_DISPLAY_HELP(0, FALSE, beep, duration);	
}

// ----------------------------------------------------------------------------

bool HelpMessages::isHelpMessageBeingDisplayed()
{
	return UI::IS_HELP_MESSAGE_BEING_DISPLAYED() == TRUE;
}

// ----------------------------------------------------------------------------

void HelpMessages::clearAll()
{
	UI::CLEAR_ALL_HELP_MESSAGES();
}

// ----------------------------------------------------------------------------
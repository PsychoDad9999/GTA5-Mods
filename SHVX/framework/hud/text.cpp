// ----------------------------------------------------------------------------

#include "text.h"

#include "../SHV/inc/natives.h"

// ----------------------------------------------------------------------------
/// <summery>Draw a text on the current frame.</summery>
/// <param name="text">Text to draw</param>
/// <param name="text">x position in pixel</param>
/// <param name="text">y position in pixel</param>
/// <param name="text">True to use green text color, false to use red text color</param>
// ----------------------------------------------------------------------------
void Text::displayTextForCurrentFrame(char* text, float x, float y, bool noError)
{
	if (text == nullptr)
		return;

	UI::SET_TEXT_FONT(7);
	UI::SET_TEXT_SCALE(0.5, 0.6);

	if (noError)
	{
		UI::SET_TEXT_COLOUR(55, 255, 55, 255);
	}
	else
	{
		UI::SET_TEXT_COLOUR(255, 55, 55, 255);
	}

	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(TRUE);
	UI::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);

	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#include "keyboardMapper.h"

#include <windows.h>

#include "keydefines.h"

// ----------------------------------------------------------------------------
/// <summery>Translate key name to virtual keycode</summery>
/// <param name="key">Key name</param>
/// <returns>Virtual keycode of key or -1</returns>
// ----------------------------------------------------------------------------
int KeyboardMapper::translateVirtualKeyCode(std::wstring key)
{
	if (contains(key))
	{
		return m_keyFinder[key];
	}
	else
	{
		return -1;
	}
}


// ----------------------------------------------------------------------------
/// <summery>Check if keycode is known for keyname</summery>
/// <param name="key">Key name</param>
/// <returns>true if keycode exist, otherwise false</returns>
// ----------------------------------------------------------------------------
bool KeyboardMapper::contains(std::wstring key)
{
	return m_keyFinder.find(key) != m_keyFinder.end();
}


// ----------------------------------------------------------------------------
/// <summery>Constructor. Creates dictionary of known keynames and keycodes</summery>
// ----------------------------------------------------------------------------
KeyboardMapper::KeyboardMapper()
{
	m_keyFinder.emplace(TEXT("none"), 0);

	m_keyFinder.emplace(TEXT("a"), KEY_A);
	m_keyFinder.emplace(TEXT("b"), KEY_B);
	m_keyFinder.emplace(TEXT("c"), KEY_C);
	m_keyFinder.emplace(TEXT("d"), KEY_D);
	m_keyFinder.emplace(TEXT("e"), KEY_E);
	m_keyFinder.emplace(TEXT("f"), KEY_F);
	m_keyFinder.emplace(TEXT("g"), KEY_G);
	m_keyFinder.emplace(TEXT("h"), KEY_H);
	m_keyFinder.emplace(TEXT("i"), KEY_I);
	m_keyFinder.emplace(TEXT("j"), KEY_J);
	m_keyFinder.emplace(TEXT("k"), KEY_K);
	m_keyFinder.emplace(TEXT("l"), KEY_L);
	m_keyFinder.emplace(TEXT("m"), KEY_M);
	m_keyFinder.emplace(TEXT("n"), KEY_N);
	m_keyFinder.emplace(TEXT("o"), KEY_O);
	m_keyFinder.emplace(TEXT("p"), KEY_P);
	m_keyFinder.emplace(TEXT("q"), KEY_Q);
	m_keyFinder.emplace(TEXT("r"), KEY_R);
	m_keyFinder.emplace(TEXT("s"), KEY_S);
	m_keyFinder.emplace(TEXT("t"), KEY_T);
	m_keyFinder.emplace(TEXT("u"), KEY_U);
	m_keyFinder.emplace(TEXT("v"), KEY_V);
	m_keyFinder.emplace(TEXT("w"), KEY_W);
	m_keyFinder.emplace(TEXT("x"), KEY_X);
	m_keyFinder.emplace(TEXT("y"), KEY_Y);
	m_keyFinder.emplace(TEXT("z"), KEY_Z);

	m_keyFinder.emplace(TEXT("0"), KEY_0);
	m_keyFinder.emplace(TEXT("1"), KEY_1);
	m_keyFinder.emplace(TEXT("2"), KEY_2);
	m_keyFinder.emplace(TEXT("3"), KEY_3);
	m_keyFinder.emplace(TEXT("4"), KEY_4);
	m_keyFinder.emplace(TEXT("5"), KEY_5);
	m_keyFinder.emplace(TEXT("6"), KEY_6);
	m_keyFinder.emplace(TEXT("7"), KEY_7);
	m_keyFinder.emplace(TEXT("8"), KEY_8);
	m_keyFinder.emplace(TEXT("9"), KEY_9);

	
	m_keyFinder.emplace(TEXT("space"), VK_SPACE);
	m_keyFinder.emplace(TEXT("esc"), VK_ESCAPE);
	m_keyFinder.emplace(TEXT("tab"), VK_TAB);
	m_keyFinder.emplace(TEXT("backspace"), VK_BACK);
	m_keyFinder.emplace(TEXT("enter"), VK_RETURN);

	m_keyFinder.emplace(TEXT("numkey0"), VK_NUMPAD0);
	m_keyFinder.emplace(TEXT("numkey1"), VK_NUMPAD1);
	m_keyFinder.emplace(TEXT("numkey2"), VK_NUMPAD2);
	m_keyFinder.emplace(TEXT("numkey3"), VK_NUMPAD3);
	m_keyFinder.emplace(TEXT("numkey4"), VK_NUMPAD4);
	m_keyFinder.emplace(TEXT("numkey5"), VK_NUMPAD5);
	m_keyFinder.emplace(TEXT("numkey6"), VK_NUMPAD6);
	m_keyFinder.emplace(TEXT("numkey7"), VK_NUMPAD7);
	m_keyFinder.emplace(TEXT("numkey8"), VK_NUMPAD8);
	m_keyFinder.emplace(TEXT("numkey9"), VK_NUMPAD9);


	m_keyFinder.emplace(TEXT("f1"), VK_F1);
	m_keyFinder.emplace(TEXT("f2"), VK_F2);
	m_keyFinder.emplace(TEXT("f3"), VK_F3);
	m_keyFinder.emplace(TEXT("f4"), VK_F4);
	m_keyFinder.emplace(TEXT("f5"), VK_F5);
	m_keyFinder.emplace(TEXT("f6"), VK_F6);
	m_keyFinder.emplace(TEXT("f7"), VK_F7);
	m_keyFinder.emplace(TEXT("f8"), VK_F8);
	m_keyFinder.emplace(TEXT("f9"), VK_F9);
	m_keyFinder.emplace(TEXT("f10"), VK_F10);
	m_keyFinder.emplace(TEXT("f11"), VK_F11);
	m_keyFinder.emplace(TEXT("f12"), VK_F12);
	m_keyFinder.emplace(TEXT("f13"), VK_F13);
	m_keyFinder.emplace(TEXT("f14"), VK_F14);
	m_keyFinder.emplace(TEXT("f15"), VK_F15);
	m_keyFinder.emplace(TEXT("f16"), VK_F16);
	m_keyFinder.emplace(TEXT("f17"), VK_F17);
	m_keyFinder.emplace(TEXT("f18"), VK_F18);
	m_keyFinder.emplace(TEXT("f19"), VK_F19);
	m_keyFinder.emplace(TEXT("f20"), VK_F20);
	m_keyFinder.emplace(TEXT("f21"), VK_F21);
	m_keyFinder.emplace(TEXT("f22"), VK_F22);
	m_keyFinder.emplace(TEXT("f23"), VK_F23);
	m_keyFinder.emplace(TEXT("f24"), VK_F24);

	m_keyFinder.emplace(TEXT("alt"), VK_MENU);
	m_keyFinder.emplace(TEXT("ctrl"), VK_CONTROL);
	m_keyFinder.emplace(TEXT("shift"), VK_SHIFT);

	m_keyFinder.emplace(TEXT(","), VK_OEM_COMMA);
	m_keyFinder.emplace(TEXT("."), VK_OEM_PERIOD);
	m_keyFinder.emplace(TEXT("-"), VK_OEM_MINUS);
}

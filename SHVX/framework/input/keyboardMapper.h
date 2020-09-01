// ----------------------------------------------------------------------------

#pragma once

#include <string>
#include <map>

// ----------------------------------------------------------------------------

class KeyboardMapper
{
public:
	int translateVirtualKeyCode(std::wstring key);

private:
	bool contains(std::wstring key);

private:
	std::map<std::wstring, int> m_keyFinder;

public:	
	KeyboardMapper();

};

// ----------------------------------------------------------------------------
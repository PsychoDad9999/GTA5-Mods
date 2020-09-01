// ----------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <string>

// ----------------------------------------------------------------------------

class Module
{
public:
	static HMODULE getModule();
	static std::wstring getCurrentModulePath();
	static std::wstring getCurrentModuleFilePath();
	static std::wstring getCurrentModuleName();

private:
	static std::wstring m_cachedModuleName;
	static std::wstring m_cachedModulePath;
	static std::wstring m_cachedModuleFilePath;

private:
	Module() {}
};

// ----------------------------------------------------------------------------
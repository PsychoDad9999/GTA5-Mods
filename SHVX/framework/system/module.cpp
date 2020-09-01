// ----------------------------------------------------------------------------

#include "module.h"

#include <string>

// ----------------------------------------------------------------------------

extern "C" IMAGE_DOS_HEADER __ImageBase; // MSVC specific, with other compilers use HMODULE from DllMain

// ----------------------------------------------------------------------------

std::wstring Module::m_cachedModulePath = TEXT("");
std::wstring Module::m_cachedModuleFilePath = TEXT("");
std::wstring Module::m_cachedModuleName = TEXT("");

// ----------------------------------------------------------------------------
/// <summery>Get module</summery>
/// <returns>Returns HMODULE</returns>
// ----------------------------------------------------------------------------
HMODULE Module::getModule()
{
	return reinterpret_cast<HMODULE>(&__ImageBase);
}


// ----------------------------------------------------------------------------
/// <summery>Get module path</summery>
/// <returns>Returns current module path with trailing slash</returns>
// ----------------------------------------------------------------------------
std::wstring Module::getCurrentModulePath()
{
	if (m_cachedModulePath.empty())
	{
		wchar_t modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));
		
		// Get name of this module
		GetModuleFileName(getModule(), modPath, sizeof(modPath));
		
		// Remove filename from path
		for (size_t i = wcslen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				modPath[i] = 0;
				break;
			}
		}

		m_cachedModulePath = modPath;
	}

	return m_cachedModulePath;
}


// ----------------------------------------------------------------------------
/// <summery>Get module file path</summery>
/// <returns>Returns current module file path</returns>
// ----------------------------------------------------------------------------
std::wstring Module::getCurrentModuleFilePath()
{
	if (m_cachedModuleFilePath.empty())
	{
		wchar_t modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));

		// Get name of this module
		GetModuleFileName(getModule(), modPath, sizeof(modPath));

		m_cachedModuleFilePath = modPath;
	}

	return m_cachedModuleFilePath;
}

// ----------------------------------------------------------------------------

std::wstring Module::getCurrentModuleName()
{
	if (m_cachedModuleName.empty())
	{
		wchar_t modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));

		// Get name of this module
		GetModuleFileName(getModule(), modPath, sizeof(modPath));

		int idx = -1;

		// Remove path from filename
		for (size_t i = wcslen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				idx = static_cast<int>(i);
				break;
			}
		}

		if (idx != -1)
		{			
			m_cachedModuleName = std::wstring(modPath).substr(idx);
		}
		else
		{
			m_cachedModuleName = modPath;
		}		
	}

	return m_cachedModuleName;
}
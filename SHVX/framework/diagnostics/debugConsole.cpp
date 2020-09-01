// ----------------------------------------------------------------------------

#include "debugConsole.h"

#include <Windows.h>

// ----------------------------------------------------------------------------

FILE* DebugConsole::m_fp = nullptr;

// ----------------------------------------------------------------------------
/// <summery>Opens a command console and redirects output</summery>
/// <remarks>Output of printf will be redirected to the command console</remarks>
// ----------------------------------------------------------------------------
void DebugConsole::init()
{
	if (AllocConsole())
	{
		freopen_s(&m_fp, "CONOUT$", "w", stdout);
	}
}


// ----------------------------------------------------------------------------
/// <summery>Close the command console and free resources</summery>
// ----------------------------------------------------------------------------
void DebugConsole::dispose()
{
	if (m_fp != nullptr)
	{
		fclose(m_fp);
		m_fp = nullptr;

		FreeConsole();
	}
}

// ----------------------------------------------------------------------------
// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>

#include "inc/main.h"
#include "framework/input/keyboard.h"
#include "script.h"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		initScript();
		scriptRegister(hModule, runScript);
		Keyboard::RegisterKeyboardHook();		
		break;

	case DLL_PROCESS_DETACH:
		scriptUnregister(hModule);
		Keyboard::UnregisterKeyboardHook();		
		cleanupScript();
        break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}


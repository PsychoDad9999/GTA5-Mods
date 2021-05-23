// ----------------------------------------------------------------------------

#include "settings.h"

#include <string>

#include <framework/system/module.h>
#include <framework/ini/fileHelper.h>
#include <framework/ini/typeConverter.h>

// ----------------------------------------------------------------------------
/// <summery>Load settings from ini file</summery>
/// <param name="settings">ModSettings struct. Will be filled with settings data from ini file</param>
/// <param name="filename">ini file name</param>
/// <remarks>The ini file must be located in the module directory</remarks>
/// <returns>Retruns true if ini file exists, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Settings::loadFromFile(Settings::ModSettings& settings, const wchar_t* filename)
{	
	const LPCWSTR groupBehavior = TEXT("Behavior");
	const LPCWSTR groupHUD = TEXT("HUD");

	std::wstring path = Module::getCurrentModulePath(); // includes trailing slash
	std::wstring fullPath = path.append(filename);

	if (!FileHelper::fileExists(fullPath))
		return false;

	LPCWSTR iniFile = fullPath.c_str();

	// Behavior settings
	settings.behavior.respawnTimeInHours = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupBehavior, TEXT("UnderwaterPickupRespawnTimeInHours"), 24, iniFile), 0, MAXINT);
	
	// HUD settings
	settings.hud.showRespawnNotification = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("ShowRespawnNotification"), 1, iniFile));
	settings.hud.showUnsupportedVersionNotification = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("ShowUnsupportedGameVersionNotification"), 1, iniFile));
	settings.hud.notificationDuration = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupHUD, TEXT("NotificationDuration"), 3000, iniFile), 1000, 10000);
	settings.hud.showPackageBlips = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("ShowHiddenPackagesOnMap"), 0, iniFile));

	return true;
}

// ----------------------------------------------------------------------------
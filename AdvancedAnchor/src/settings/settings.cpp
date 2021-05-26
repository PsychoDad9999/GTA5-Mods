// ----------------------------------------------------------------------------

#include "settings.h"

#include <string>

#include "framework/system/module.h"
#include "framework/input/keyboardMapper.h"
#include "framework/ini/fileHelper.h"
#include "framework/ini/typeConverter.h"

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
	const LPCWSTR groupKeys = TEXT("Keys");
	const LPCWSTR groupVehicles = TEXT("Vehicles");
	const LPCWSTR groupHUD = TEXT("HUD");
	const LPCWSTR groupAudio = TEXT("Audio");
	const LPCWSTR groupMisc = TEXT("Misc");

	std::wstring path = Module::getCurrentModulePath(); // includes trailing slash
	std::wstring fullPath = path.append(filename);

	if (!FileHelper::fileExists(fullPath))
		return false;

	LPCWSTR iniFile = fullPath.c_str();

	// Behavior settings
	settings.behavior.autoDeployAnchorWhenLeaveVehicle = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupBehavior, TEXT("AutoDeployAnchorWhenLeaveVehicle"), 1, iniFile));
	settings.behavior.autoRetrieveAnchorWhenGetInVehicle = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupBehavior, TEXT("AutoRetrieveAnchorWhenGetInVehicle"), 1, iniFile));
	settings.behavior.anchorAutoRemovalDistance = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupBehavior, TEXT("AnchorAutoRemovalDistance"), 2000, iniFile), 25, MAXINT, 0);
	
	KeyboardMapper keyMapper;

	const int BUFFER_SIZE = 256;
	wchar_t buffer[BUFFER_SIZE];

	// keys settings
	int strLen = GetPrivateProfileString(groupKeys, TEXT("AnchorKey"), TEXT("NONE"), buffer, BUFFER_SIZE, iniFile);

	if (strLen > 0)
	{			
		settings.input.anchorKey = keyMapper.translateVirtualKeyCode(TypeConverter::strToKeyNameSafe(buffer));
	}
	
	// vehicle settings
	settings.vehicles.allowBoat = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupVehicles, TEXT("CanAnchorBoat"), 1, iniFile));
	settings.vehicles.allowJetski = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupVehicles, TEXT("CanAnchorJetski"), 1, iniFile));
	settings.vehicles.allowSubmersible = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupVehicles, TEXT("CanAnchorSub"), 1, iniFile));
	settings.vehicles.allowSeaPlanes = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupVehicles, TEXT("CanAnchorSeaPlane"), 1, iniFile));
	settings.vehicles.allowSeaHelicopters = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupVehicles, TEXT("CanAnchorSeaHelicopter"), 1, iniFile));

	// HUD settings
	settings.hud.allowNotifications = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("AllowNotifications"), 1, iniFile));
	settings.hud.notificationDuration = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupHUD, TEXT("NotificationDuration"), 3000, iniFile), 1000, 10000);
	settings.hud.allowHelpMessages = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("AllowHelpMessages"), 1, iniFile));
	settings.hud.showAnchoredVehiclesOnMap = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("ShowAnchoredVehiclesOnMap"), 1, iniFile));
	settings.hud.useShortRangeMapMarkers = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupHUD, TEXT("UseShortRangeMapMarkers"), 1, iniFile));

	// Audio settings
	settings.audio.allowAnchorSounds = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupAudio, TEXT("AllowAnchorSoundEffects"), 1, iniFile));
	settings.audio.volumeAnchorBoat = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupAudio, TEXT("VolumeAnchorBoat"), 100, iniFile), 0, 200);
	settings.audio.volumeAnchorJetski = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupAudio, TEXT("VolumeAnchorJetski"), 100, iniFile), 0, 200);
	settings.audio.volumeAnchorSub = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupAudio, TEXT("VolumeAnchorSub"), 100, iniFile), 0, 200);
	settings.audio.volumeAnchorSeaPlane = TypeConverter::intToULongSafe(GetPrivateProfileInt(groupAudio, TEXT("VolumeAnchorSeaPlane"), 100, iniFile), 0, 200);

	// MISC settings	
	settings.misc.allowHoldKeyToEnterSubmersible = TypeConverter::intToBoolSafe(GetPrivateProfileInt(groupMisc, TEXT("AllowHoldKeyToEnterSubmersible"), 1, iniFile));

	return true;
}

// ----------------------------------------------------------------------------
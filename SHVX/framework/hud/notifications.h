// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <map>

// ----------------------------------------------------------------------------

class Notifications
{
private:
	struct Notification
	{
		// start timestamp of notification in milliseconds
		ULONGLONG notificationStartTime;
		// display time in milliseconds
		ULONG maxDisplayTime;
		// Native Notification handle
		int notificationHandle;

		Notification() : Notification(0, 0, 0) {}
		Notification(ULONGLONG startTime, ULONG displayTime, int handle) : notificationStartTime(startTime), maxDisplayTime(displayTime), notificationHandle(handle) {}
	};


public:
	// this should be called on every frame
	static void update();

	static void showNotification(char* text, ULONG duration, int id);

	static bool isNotificationDisplayed();

private:
	static void removeNotification(int notificationHandle);

private:
	// current timestamp in milliseconds
	// will be updated in function update
	static ULONGLONG m_currentTimestamp;

	static std::map<int, Notification> m_notificationList;

private:
	Notifications();	
};

// ----------------------------------------------------------------------------
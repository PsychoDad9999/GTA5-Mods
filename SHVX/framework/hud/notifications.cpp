// ----------------------------------------------------------------------------

#include "notifications.h"

#include "../SHV/inc/natives.h"

// ----------------------------------------------------------------------------

const int MAX_NOTIFICATION_TIME = 10000; // 10 Seconds

// Current timestamp in milliseconds
// Will be updated in function update
ULONGLONG Notifications::m_currentTimestamp = 0;

std::map<int, Notifications::Notification> Notifications::m_notificationList;


// ----------------------------------------------------------------------------
/// <summery>Update notifications. This must be called periodically.</summery>
// ----------------------------------------------------------------------------
void Notifications::update()
{
	// update current time
	m_currentTimestamp = GetTickCount64();

	// check if we need to clear any notification
	std::map<int, Notifications::Notification>::iterator it = m_notificationList.begin();
	
	while (it != m_notificationList.end()) 
	{
		Notification notification = it->second;

		if (m_currentTimestamp > notification.notificationStartTime + notification.maxDisplayTime)
		{
			// remove game notification
			removeNotification(notification.notificationHandle);

			// remove from notification list
			it = m_notificationList.erase(it);
		}
		else
		{
			it++;
		}
	}
}


// ----------------------------------------------------------------------------
/// <summery>Show a natification.</summery>
/// <param name="text">Notification text</param>
/// <param name="duration">Notification duration in milliseconds</param>
/// <param name = "duration">Notification id. If a notification with the same id is already shown, the old notification will be removed first</param>
// ----------------------------------------------------------------------------
void Notifications::showNotification(char* text, ULONG duration, int id)
{
	if (m_currentTimestamp == 0)
	{
		// update has not been called yet
		m_currentTimestamp = GetTickCount64();
	}


	if (m_notificationList.find(id) != m_notificationList.end())
	{
		//remove old notification with this id
		removeNotification(m_notificationList[id].notificationHandle);
	}
		
	UI::SET_TEXT_CENTRE(TRUE);
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);

	int notificationHandle = UI::_DRAW_NOTIFICATION(FALSE, FALSE);	

	// add notifcation to list
	Notification notification = Notification(m_currentTimestamp, duration > MAX_NOTIFICATION_TIME ? MAX_NOTIFICATION_TIME : duration, notificationHandle);

	m_notificationList[id] = notification;	
}


// ----------------------------------------------------------------------------
/// <summery>Check if any notification is currently shown</summery>
/// <returns>Retruns true if a notification is shown, otherwise false</returns>
// ----------------------------------------------------------------------------
bool Notifications::isNotificationDisplayed()
{
	return !m_notificationList.empty();
}


// ----------------------------------------------------------------------------
/// <summery>Remove a natification.</summery>
/// <param name="notificationHandle">Native notification handle</param>
// ----------------------------------------------------------------------------
void Notifications::removeNotification(int notificationHandle)
{
	// remove game notification
	UI::_REMOVE_NOTIFICATION(notificationHandle);
}


// ----------------------------------------------------------------------------
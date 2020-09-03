// ----------------------------------------------------------------------------

#include "hiddenPackagesManager.h"

#include <framework/hud/blip.h>
#include <framework/world/world.h>

// ----------------------------------------------------------------------------


Vector3 v(float x, float y, float z)
{
	return Vector3({ x, 0, y, 0, z, 0 });
}


HiddenPackagesManager::HiddenPackagesManager()
{
	m_hiddenPackages =
	{
		 HiddenPackage(0, v(-1029, 6517, -21),	v(-998.1893, 6538.748, -31.8168)),
		 HiddenPackage(1, v(3265, 6407, -47),	v(3267.372, 6408.691, -49.36513)),
		 HiddenPackage(2, v(4215, 3603, -48),	v(4241.071, 3600.492, -46.81019)),
		 HiddenPackage(3, v(-3247, 3669, -28),	v(-3242.5, 3667.486, -30.78001)),
		 HiddenPackage(4, v(-3192, 3029, -35),	v(-3178.462, 3038.094, -35.22701)),
		 HiddenPackage(5, v(-2838, -446, -38),	v(-2833.384, -468.0155, -20.40657)),
		 HiddenPackage(6, v(3156, -307, -17),	v(3166.303, -309.2426, -13.46703)),
		 HiddenPackage(7, v(2695, -1334, -26),	v(2690.049, -1407.16, -23.1385)),
		 HiddenPackage(8, v(264, -2280, -8),	v(263.0383, -2278.808, -5.90399)),
		 HiddenPackage(9, v(1804, -2993, -46),	v(1779.454, -2984.549, -47.8651)),
		 HiddenPackage(10, v(-131, -2867, -10),	v(-134.8904, -2870.548, -21.0327)),
		 HiddenPackage(11, v(3595, 3041, -15),	v(3881.517, 3042.363, -24.8541)),
	};
}


void HiddenPackagesManager::createPackageBlips()
{
	const int SPRITE_ID = 272;
		
	MapBlip::AddBlipForCoordinate(v(-998.1893, 6538.748, -31.8168), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(3267.372, 6408.691, -49.36513), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(4241.071, 3600.492, -46.81019), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(-3242.5, 3667.486, -30.78001), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(-3178.462, 3038.094, -35.22701), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(-2833.384, -468.0155, -20.40657), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(3166.303, -309.2426, -13.46703), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(2690.049, -1407.16, -23.1385), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(263.0383, -2278.808, -5.90399), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(1779.454, -2984.549, -47.8651), true, SPRITE_ID);
	MapBlip::AddBlipForCoordinate(v(-134.8904, -2870.548, -21.0327), true, SPRITE_ID);
	
	// This Pickup is not activated by the game
	// MapBlip::AddBlipForCoordinate(3881.517, 3042.363, -24.8541, true, SPRITE_ID);
}


HiddenPackage* HiddenPackagesManager::getPackageWithClosestActivationPoint(Vector3 location, double* outDistance)
{
	int minIdx = -1;
	double minDistance = -1;

	
	for (int arrayIndex = 0; arrayIndex < m_hiddenPackages.size() ; arrayIndex++)
	{
		double distance = World::getDistance(location, m_hiddenPackages[arrayIndex].scriptActivationLocation, false);

		if (minIdx == -1)
		{
			minDistance = distance;
			minIdx = arrayIndex;
		}
		else
		{
			if (distance < minDistance)
			{
				minDistance = distance;
				minIdx = arrayIndex;
			}
		}
	}

	*outDistance = minDistance;
	return &m_hiddenPackages[minIdx];
}


void HiddenPackagesManager::purge()
{
	for (int arrayIndex = 0; arrayIndex < m_hiddenPackages.size(); arrayIndex++)
	{
		m_hiddenPackages[arrayIndex].isPickupInRange = false;
		m_hiddenPackages[arrayIndex].nextSpawnTime = DateTime(DateTime::TIME_NOT_SET);
	}
}
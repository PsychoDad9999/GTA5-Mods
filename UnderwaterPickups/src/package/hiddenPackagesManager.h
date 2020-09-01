// ----------------------------------------------------------------------------

#pragma once

#include <array>

#include "hiddenPackage.h"

// ----------------------------------------------------------------------------

class HiddenPackagesManager
{

public:	
	HiddenPackage* getPackageWithClosestActivationPoint(Vector3 location, double* outDistance);

	void purge();

	void createPackageBlips();

public:
	HiddenPackagesManager();


private:
	std::array<HiddenPackage, 12> m_hiddenPackages;
};
// ----------------------------------------------------------------------------

#pragma once

#include <Windows.h>

// ----------------------------------------------------------------------------

class Physics
{
public:
	static double calculateBreakingDistanceInMeters(double speedKmph);
	static double calculateEmergencyBreakingDistanceInMeters(double speedKmph);

	static ULONGLONG calculateTotalTimeOfBreakingInMilliseconds(double startSpeedInKmph, double breakingDistanceInMeters);

	static double mpsToKmph(double speedMps);
	static double kmphToMps(double speedKmph);



private:
	Physics() {}
};

// ----------------------------------------------------------------------------
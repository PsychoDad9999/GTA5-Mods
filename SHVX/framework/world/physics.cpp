// ----------------------------------------------------------------------------

#include "physics.h"

#include <math.h>

// ----------------------------------------------------------------------------
/// <summery>Calculate breaking distance in meters</summery>
/// <param name="speedKmph">Speed in Km/h</param>
/// <remarks>Calculation is based on average breaking distance on dry road</remarks>
/// <returns>Retruns breaking distance in meters</returns>
// ----------------------------------------------------------------------------
double Physics::calculateBreakingDistanceInMeters(double speedKmph)
{
	return pow(speedKmph / 10.f, 2);
}


// ----------------------------------------------------------------------------
/// <summery>Calculate emergency breaking distance in meters</summery>
/// <param name="speedKmph">Speed in Km/h</param>
/// <remarks>Calculation is based on average emergency breaking distance on dry road</remarks>
/// <returns>Retruns breaking distance in meters</returns>
// ----------------------------------------------------------------------------
double Physics::calculateEmergencyBreakingDistanceInMeters(double speedKmph)
{
	return calculateBreakingDistanceInMeters(speedKmph) / 2.f;
}


// ----------------------------------------------------------------------------
/// <summery>Convert Meters per Second to Km/h</summery>
/// <param name="speedMps">Speed in meters per second</param>
/// <returns>Retruns speed in Km/h</returns>
// ----------------------------------------------------------------------------
double Physics::mpsToKmph(double speedMps)
{
	return 3.6 * speedMps;

}


// ----------------------------------------------------------------------------
/// <summery>Convert Km/h to meters per second</summery>
/// <param name="speedKmph">Speed in Km/h</param>
/// <returns>Retruns speed in meters per second</returns>
// ----------------------------------------------------------------------------
double Physics::kmphToMps(double speedKmph)
{
	return speedKmph / 3.6;
}


// ----------------------------------------------------------------------------
/// <summery>Calculate total time of breaking in milliseconds</summery>
/// <param name="startSpeedInKmph">Speed at the beginning of breaking in Km/h</param>
/// <param name="breakingDistanceInMeters">Breaking distance in meters</param>
/// <returns>Retruns total time of breaking in milliseconds</returns>
// ----------------------------------------------------------------------------
ULONGLONG Physics::calculateTotalTimeOfBreakingInMilliseconds(double startSpeedInKmph, double breakingDistanceInMeters)
{
	double breakingTimeInSeconds = (2 * breakingDistanceInMeters) / kmphToMps(fabs(startSpeedInKmph));

	return static_cast<ULONGLONG>(breakingTimeInSeconds * 1000);
}

// ----------------------------------------------------------------------------

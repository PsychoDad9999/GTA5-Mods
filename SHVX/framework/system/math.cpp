// ----------------------------------------------------------------------------

#include "math.h"

#include <cmath>

// ----------------------------------------------------------------------------
/// <summery>Normalize Value</summery>
/// <param name="value">Integer value</param>
/// <param name="min">Minimum value</param>
/// <param name="value">Maximum value</param>
/// <returns>Returns a normalized int value between Min and Max</returns>
// ----------------------------------------------------------------------------
int Math::normalize(int value, int min, int max)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;

	return value;
}

// ----------------------------------------------------------------------------
/// <summery>Get Nearest Value</summery>
/// <param name="value">value</param>
/// <param name="min">Minimum value</param>
/// <param name="value">Maximum value</param>
/// <returns>Returns the nearest value between Min and Max</returns>
// ----------------------------------------------------------------------------

float Math::getNearestValue(float value, float min, float max)
{
	float diff;

	if (min == max)
	{
		return min;
	}

	diff = (max - min);

	value = (value - (static_cast<int>(round(((value - min) / diff))) * diff));

	if (value < min)
	{
		value = (value + diff);
	}

	return value;
}
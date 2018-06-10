#include <Arduino.h>
#include "util.hpp"

// Assuming that angle system is based on 0 to 360.
float calc_angle_diff(float a1, float a2)
{
	float c1 = a1 - a2;
	float c2 = a1 - a2 + 360;
	float c3 = a1 - a2 - 360;
	if (abs(c1) < abs(c2) && abs(c1) < abs(c3))
		return c1;
	if (abs(c2) < abs(c3))
		return c2;
	else 
		return c3;
}

float limit(float input, float lower, float upper)
{
	if (input < lower) 
		return lower;
	if (input > upper)
		return upper;
	return input;
}

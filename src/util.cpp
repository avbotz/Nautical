#include <Arduino.h>

#include "util.hpp"


/*
 * Angle system is based on 0 - 360.
 * The edge case is at 0/360, where the difference is 0.
 */
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

/*
 * Prevents "party" mode by limiting a value between two values.
 */
float limit(float p, float c1, float c2)
{
	if (p < c1) return c1;
	if (p > c2) return c2;
	return p;
}

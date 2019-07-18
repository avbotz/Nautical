#include <Arduino.h>
#include "util.hpp"


float angle_difference(float a1, float a2)
{
	// For [0, 360].
	/*
	   float c1 = a1 - a2;
	   float c2 = a1 - a2 + 360.;
	   float c3 = a1 - a2 - 360.;
	   if (abs(c1) < abs(c2) && abs(c1) < abs(c3))
	   return c1;
	   if (abs(c2) < abs(c3))
	   return c2;
	   else 
	   return c3;
	*/

	// For [-180, 180].
	float b1 = a1-a2;
	if (fabs(b1) > 180.)
	{
		if (a1 < a2)
			a1 += 360.;
		else 
			a2 += 360.;
		b1 = a1-a2;
	}
	return b1;
}

float angle_add(float a1, float add)
{
	float temp = a1 + add;
	if (temp > 180.0)
		return temp - 360.0;
	else if (temp < -180.)
		return temp + 360.0;

	return temp;
}

float limit(float input, float lower, float upper)
{
	if (input < lower) 
		return lower;
	if (input > upper)
		return upper;
	return input;
}

float limit(float input, float min)
{
	if (input < 0. && input > -1.*min)
		return -1*min;
	if (input > 0. && input < min)
		return min;
	return input;
}

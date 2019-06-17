#include <Arduino.h>
#include "util.hpp"


float angle_difference(float a1, float a2)
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

float angle_add(float a1, float add)
{
	float temp = a1 + add;
	if (temp > 360.0)
		return temp - 360.0;
	else if (temp < 0)
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
	if (input < 0 && input > -1*min)
		return -1*min;
	if (input > 0 && input < min)
		return min;
	return input;
}

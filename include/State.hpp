#ifndef STATE_HPP
#define STATE_HPP 

#include <Arduino.h>
#include "Streaming.h"

struct State 
{
	/*
	 * D | +/- 	
	 * X = forward/backward
	 * Y = right/left 
	 * Z = down/up
	 * Yaw, Pitch, and Roll are obvious.
	 */
	float x, y, z, yaw, pitch, roll;
	State() : x(0), y(0), z(0), yaw(0), pitch(0), roll(0) {}

	void read();
	void print();
};

#endif 

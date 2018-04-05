#ifndef STATE_HPP
#define STATE_HPP 

#include <Arduino.h>
#include "streaming.h"

struct State 
{
	/*
	 * D | +/- 	
	 * X = forward/backward
	 * Y = right/left 
	 * Z = down/up
	 * Yaw, Pitch, and Roll are obvious.
	 * Acceleration: 
	 * Surge = forward
	 * Sway = right/left
	 * Heave = down/up
	 */
	float x, y, z;
	float yaw, pitch, roll;
	float sway, heave, surge;
	uint_fast8_t hstatus;

	State() : x(0), y(0), z(0), yaw(0), pitch(0), roll(0),
		sway(0), heave(0), surge(0), hstatus(0) {}
	State(float sx, float sy, float sz, float syw, float sp, float sr) :
		x(sx), y(sy), z(sz), yaw(syw), pitch(sp), roll(sr) {}

	void read();
	void print();
	void printfull();
};

#endif 

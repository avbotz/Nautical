#include <Arduino.h>
#include "ahrs/ahrs.h"
#include "streaming.h"
#include "state.hpp"

void State::read()
{
	this->x = Serial.parseFloat();
	this->y = Serial.parseFloat();
	this->z = Serial.parseFloat();
	this->yaw 	= Serial.parseFloat();
	this->pitch = Serial.parseFloat();
	this->roll 	= Serial.parseFloat();
}

void State::print()
{
	Serial << this->x << " " << this->y << " " << this->z << " " << 
		this->yaw << " " << this->pitch << " " << this->roll << '\n';
}

void State::print_complete()
{
	Serial << this->x << " " << this->y << " " << this->z << " " << 
		this->yaw << " " << this->pitch << " " << this->roll << " " << 
		this->ax << " " << this->ay << " " << this->az << '\n';
}

void compute_state(State &state, State &desired, unsigned long start, float p)
{
	ahrs_att_update();
	state.yaw 		= ahrs_att((enum att_axis)(YAW));
	state.pitch 	= ahrs_att((enum att_axis)(PITCH));
	state.roll 		= ahrs_att((enum att_axis)(ROLL));

	unsigned long end = micros();
	float dt = (float)(end - start)/1000000.0f;

	float dstate[3] = { 0.0f };
	dstate[0] = desired.x - state.x;
	dstate[1] = desired.y - state.y;
	dstate[2] = desired.z - state.z;

	float kdir[3] = { 0.0f };
	for (int i = 0; i < 3; i++) 
		if (dstate[i] > 0.01f) 
			kdir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;

	// Using sub-units, mapping power to distance (time * p)
	state.x += kdir[0] * p * dt;
	state.y += kdir[0] * p * dt;
	state.z += kdir[0] * p * dt;
}

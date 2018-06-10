#include <Arduino.h>

#include "ahrs/ahrs.h"
#include "streaming.h"
#include "state.hpp"
#include "io.hpp"

void State::read()
{
	this->x = Serial.parseFloat();
	this->y = Serial.parseFloat();
	this->z = Serial.parseFloat();
	this->yaw = Serial.parseFloat();
	this->pitch = Serial.parseFloat();
	this->roll = Serial.parseFloat();
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

void reset_state(State &state)
{
	state.x = 0.0f;
	state.y = 0.0f;
	state.z = 0.0f;
}

void compute_state(State &state, float dstate[DOF], float pid[DOF], float p, uint32_t start)
{
	ahrs_att_update();
	state.yaw = ahrs_att((enum att_axis)(YAW));
	state.pitch = ahrs_att((enum att_axis)(PITCH));
	state.roll = ahrs_att((enum att_axis)(ROLL));
	
	uint32_t end = micros();
	float dt = (float)(end - start)/1000000.0f;

	float dstate[3] = { 0.0f };
	dstate[0] = desired.x - state.x;
	dstate[1] = desired.y - state.y;
	dstate[2] = desired.z - state.z;

	float kdir[3] = { 0.0f };
	for (int i = 0; i < 3; i++) 
		kdir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;

	// Using sub-units, mapping power to distance (time * p)
	state.x += kdir[0] * p * dt;
	state.y += kdir[1] * p * dt;
	// state.z += kdir[2] * p * dt;
	state.z = analogRead(NPIN);
}

void compute_initial_state(State &state)
{
	ahrs_att_update();
	state.iax = ahrs_accel((enum accel_axis)(SURGE));
	state.iay = ahrs_accel((enum accel_axis)(SWAY));
	state.iaz = ahrs_accel((enum accel_axis)(HEAVE));
	// state.z = analogRead(NPIN);
}

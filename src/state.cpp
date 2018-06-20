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

uint32_t compute_state(State &state, float pid[DOF], float p, uint32_t start)
{
	ahrs_att_update();
	state.yaw = ahrs_att((enum att_axis)(YAW));
	state.pitch = ahrs_att((enum att_axis)(PITCH));
	state.roll = ahrs_att((enum att_axis)(ROLL));
	
	// Using sub-units, mapping power to distance (time * p)
	float dt = (float)(micros() - start)/(float)(1000000);
	state.x += pid[0] * p * dt;
	state.y += pid[1] * p * dt;
	// state.z += pid[2] * p * dt;
	state.z = (analogRead(NPIN) - 230.0f)/60.0f;
	return micros();
}

void compute_initial_state(State &state)
{
	ahrs_att_update();
	state.yaw = ahrs_att((enum att_axis)(YAW));
	state.pitch = ahrs_att((enum att_axis)(PITCH));
	state.roll = ahrs_att((enum att_axis)(ROLL));
	state.iax = ahrs_accel((enum accel_axis)(SURGE));
	state.iay = ahrs_accel((enum accel_axis)(SWAY));
	state.iaz = ahrs_accel((enum accel_axis)(HEAVE));
	// state.z = analogRead(NPIN);
}

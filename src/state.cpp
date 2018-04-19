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

void reset_state(State &state)
{
	state.x = 0.0f;
	state.y = 0.0f;
	state.z = 0.0f;
	compute_initial_state(state);
}

void compute_state(State &state, float &vx, float &vy, float &vz, unsigned long start)
{
	ahrs_att_update();
	state.yaw 		= ahrs_att((enum att_axis)(YAW));
	state.pitch 	= ahrs_att((enum att_axis)(PITCH));
	state.roll 		= ahrs_att((enum att_axis)(ROLL));
	state.ax	 	= ahrs_accel((enum accel_axis)(SURGE)) - state.iax;
	state.ay 		= ahrs_accel((enum accel_axis)(SWAY)) - state.iay;
	state.az 		= ahrs_accel((enum accel_axis)(HEAVE)) - state.iaz;

	float dt = (float)(micros() - start)/(float)(1000000);

	vx += state.ax * dt;
	vy += state.ay * dt;
	vz += state.az * dt;

	state.x += vx * dt;
	state.y += vy * dt;
	state.z = analogRead(NPIN);
}

void compute_initial_state(State &state)
{
	ahrs_att_update();
	state.iax = ahrs_accel((enum accel_axis)(SURGE));
	state.iay = ahrs_accel((enum accel_axis)(SWAY));
	state.iaz = ahrs_accel((enum accel_axis)(HEAVE));
	state.z = analogRead(NPIN);
}

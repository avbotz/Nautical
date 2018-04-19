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
	state.x = 0.f;
	state.y = 0.f;
	state.z = 0.f;
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

	unsigned long end = micros();
	float td = (double)(end - start)/(double)(1000000.f);

	vx += state.ax * td;
	vy += state.ay * td;
	vz += state.az * td;

	state.x += vx * td;
	state.y += vy * td;
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

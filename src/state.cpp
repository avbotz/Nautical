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

void compute_state(State &state, float &vx, float &vy, float &vz, unsigned long start)
{
	ahrs_att_update();
	state.yaw 		= ahrs_att((enum att_axis)(YAW));
	state.pitch 	= ahrs_att((enum att_axis)(PITCH));
	state.roll 		= ahrs_att((enum att_axis)(ROLL));
	state.ax	 	= ahrs_accel((enum accel_axis)(SURGE));
	state.ay 		= ahrs_accel((enum accel_axis)(SWAY));
	state.az 		= ahrs_accel((enum accel_axis)(HEAVE));

	unsigned long end = micros();
	float td = (double)(end - start)/(double)(1000000.f);

	vx += state.ax * td;
	vy += state.ay * td;
	vz += state.az * td;

	state.x += vx * td;
	state.y += vy * td;
	state.z += vz * td;
}

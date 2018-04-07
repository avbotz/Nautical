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
		this->surge << " " << this->sway << " " << this->heave << '\n';
}

void compute_state(State &state, unsigned long start)
{
	ahrs_att_update();
	state.yaw 		= ahrs_att((enum att_axis)(YAW));
	state.pitch 	= ahrs_att((enum att_axis)(PITCH));
	state.roll 		= ahrs_att((enum att_axis)(ROLL));
	state.surge 	= ahrs_accel((enum accel_axis)(SURGE));
	state.sway 		= ahrs_accel((enum accel_axis)(SWAY));
	state.heave 	= ahrs_accel((enum accel_axis)(HEAVE));

	float td = (float)(micros() - start)/(float)(1000000);

	state.x += 0.5 * state.surge * td * td;
	state.y += 0.5 * state.sway * td * td;
	state.z = analogRead(NPIN);
	//state.z += 0.5 * state.heave * td * td;
}

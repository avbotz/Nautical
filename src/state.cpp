#include <Arduino.h>
#include "streaming.h"

#include "ahrs/ahrs.h"
#include "state.hpp"
#include "io.hpp"


void State::read()
{
	for (int i = 0; i < DOF; i++)
		this->axis[i] = Serial.parseFloat();
}

void State::print()
{
	Serial << this->axis[X] << " " << this->axis[Y] << " " << this->axis[Z] << " " <<
		this->axis[YAW] << " " << this->axis[PITCH] << " " << this->axis[ROLL] << '\n';
}

void State::print_complete()
{
	Serial << this->axis[X] << " " << this->axis[Y] << " " << this->axis[Z] << " " <<
		this->axis[YAW] << " " << this->axis[PITCH] << " " << this->axis[ROLL] << 
		this->accel[SURGE] << this->accel[SWAY] << this->accel[HEAVE] << '\n';
}

void reset_state(State &state)
{
	state.x = 0.0f;
	state.y = 0.0f;
	state.z = 0.0f;
	// compute_initial_state(state);
}

uint32_t compute_state(State &state, State &desired, float p, uint32_t start)
{
	// Get angle data in degrees from IMU.
	ahrs_att_update();
	state.axis[YAW]		= ahrs_att((enum att_axis)(YAW));
	state.axis[PITCH] 	= ahrs_att((enum att_axis)(PITCH));
	state.axis[ROLL]	= ahrs_att((enum att_axis)(ROLL));

	// Calculate time difference, needed for sub-units.
	uint32_t end = micros();
	float dt = (float)(end - start)/(float)(1000000);

	// Calculate state difference.
	float dstate[3] = { 0.0f };
	dstate[0] = desired.x - state.x;
	dstate[1] = desired.y - state.y;
	dstate[2] = desired.z - state.z;

	float kdir[3] = { 0.0f };
	for (int i = 0; i < 3; i++) 
		if (dstate[i] > 0.01f) 
			kdir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;

	// Using sub-units, mapping power to distance (time * p).
	state.x += kdir[0] * p * dt;
	state.y += kdir[1] * p * dt;
	state.z += kdir[2] * p * dt;
	// state.z = analogRead(NPIN);
	
	return end;
}

void compute_initial_state(State &state)
{
	ahrs_att_update();
	state.iax = ahrs_accel((enum accel_axis)(SURGE));
	state.iay = ahrs_accel((enum accel_axis)(SWAY));
	state.iaz = ahrs_accel((enum accel_axis)(HEAVE));
	// state.z = analogRead(NPIN);
}

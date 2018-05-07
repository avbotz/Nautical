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
		this->axis[Yaw] << " " << this->axis[Pitch] << " " << this->axis[Roll] << '\n';
}

void State::print_complete()
{
	Serial << this->axis[X] << " " << this->axis[Y] << " " << this->axis[Z] << " " <<
		this->axis[Yaw] << " " << this->axis[Pitch] << " " << this->axis[Roll] << " " <<
		this->accel[Surge] << " " << this->accel[Sway] << " " << this->accel[Heave] << '\n';
}

void reset_state(State &state)
{
	for (int i = 0; i < MOVE_DOF; i++)
		state.axis[i] = 0.0f;
	// compute_initial_state(state);
}

uint32_t compute_state(State &state, State &desired, float p, uint32_t start)
{
	// Get angle data in degrees from IMU.
	ahrs_att_update();
	state.axis[Yaw]		= ahrs_att((enum att_axis)(YAW));
	state.axis[Pitch]	= ahrs_att((enum att_axis)(PITCH));
	state.axis[Roll]	= ahrs_att((enum att_axis)(ROLL));

	// Calculate time difference, needed for sub-units.
	uint32_t end = micros();
	float dt = (float)(end - start)/(float)(1000000);

	// Calculate state difference.
	float dstate[MOVE_DOF] = { 0.0f };
	for (int i = 0; i < MOVE_DOF; i++)
		dstate[i] = desired.axis[i] - state.axis[i];

	// Calculate direction to change sub-units.
	float kdir[MOVE_DOF] = { 0.0f };
	for (int i = 0; i < MOVE_DOF; i++) 
		if (dstate[i] > 0.01f) 
			kdir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;

	// Using sub-units, mapping power to distance (time * p).
	for (int i = 0; i < MOVE_DOF; i++)
		state.axis[i] += kdir[i] * p * dt;
	// state.z = analogRead(NPIN);
	
	return end;
}

void compute_initial_state(State &state)
{
	ahrs_att_update();
	state.initial_accel[Surge]	= ahrs_accel((enum accel_axis)(SURGE));
	state.initial_accel[Sway]	= ahrs_accel((enum accel_axis)(SWAY));
	state.initial_accel[Heave]	= ahrs_accel((enum accel_axis)(HEAVE));
	// state.z = analogRead(NPIN);
}

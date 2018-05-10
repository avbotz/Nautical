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

/*
 * This implementation to compute state uses sub-units, which are power based. 
 */
uint32_t compute_state(State &state, float dstate[DOF], float p, uint32_t start)
{
	// Get angle data in degrees from IMU.
	ahrs_att_update();
	state.axis[Yaw]		= ahrs_att((enum att_axis)(YAW));
	state.axis[Pitch]	= ahrs_att((enum att_axis)(PITCH));
	state.axis[Roll]	= ahrs_att((enum att_axis)(ROLL));

	// Calculate direction to change sub-units.
	float kdir[MOVE_DOF] = { 0.0f };
	for (int i = 0; i < MOVE_DOF; i++) 
		if (dstate[i] > 0.01f) 
			kdir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;

	// Calculate time difference, needed for sub-units.
	uint32_t end = micros();
	float dt = (float)(end - start)/(float)(1000000);

	// Using sub-units, mapping power to distance (time * p).
	for (int i = 0; i < MOVE_DOF; i++)
		state.axis[i] += kdir[i] * p * dt;
	// state.z = analogRead(NPIN);
	
	return end;
}

/*
 * This implementation to compute state uses accelerometer data. 
 */
uint32_t compute_state(State &state, float velocities[MOVE_DOF], uint32_t start)
{
	// Get angle data in degrees from IMU.
	ahrs_att_update();
	state.axis[Yaw]		= ahrs_att((enum att_axis)(YAW));
	state.axis[Pitch]	= ahrs_att((enum att_axis)(PITCH));
	state.axis[Roll]	= ahrs_att((enum att_axis)(ROLL));

	// Get acceleration data in m/s^2.
	// Order is X, Y, Z accelerations.
	state.accel[Surge]	= ahrs_accel((enum accel_axis)(SURGE)) - state.initial_accel[Surge];
	state.accel[Sway]	= ahrs_accel((enum accel_axis)(SWAY)) - state.initial_accel[Sway];
	state.accel[Heave]	= ahrs_accel((enum accel_axis)(HEAVE)) - state.initial_accel[Heave];

	// Add acclerations to velocities.
	for (int i = 0; i < MOVE_DOF; i++)
		velocities[i] += state.accel[i];

	// Calculate time difference, add distance to X, Y, and Z.
	uint32_t end = micros();
	float dt = (float)(end - start)/(float)(1000000);
	for (int i = 0; i < MOVE_DOF; i++)
		state.axis[i] += velocities[i] * dt;

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

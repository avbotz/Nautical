#ifndef STATE_HPP
#define STATE_HPP

#include "config.h"

struct State
{
	/*
	 * D | +/-
	 * X = forward/backward
	 * Y = right/left
	 * Z = down/up
	 * Yaw, Pitch, and Roll are obvious.
	 * Surge (AX) 	= forward/backward
	 * Sway  (AY)	= right/left
	 * Heave (AZ) 	= down/up
	 */
	float axis[DOF];
	float accel[MOVE_DOF];
	float initial_accel[MOVE_DOF];
	State()
	{
		for (int i = 0; i < DOF; i++)
			axis[i] = 0.0f;
		for (int i = 0; i < MOVE_DOF; i++)
		{
			accel[i] = 0.0f;
			initial_accel[i] = 0.0f;
		}
	}

	void read();
	void print();
	void print_complete();
};

void reset_state(struct State &state);
void compute_initial_state(struct State &state);
uint32_t compute_state(struct State &state, float dstate[DOF], float p, uint32_t start);

#endif

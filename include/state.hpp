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
	State() : axis({ 0.0f }), accel({ 0.0f }), initial_accel(0.0f) {}

	void read();
	void print();
	void print_complete();
};

void reset_state(struct State &state);
void compute_initial_state(struct State &state);
uint32_t compute_state(struct State &state, struct State &desired, float p, uint32_t start);

#endif

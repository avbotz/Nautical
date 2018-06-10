#include "streaming.h"
#include "pid.hpp"
#include "motor.hpp"
#include "util.hpp"


void set_motor(int id, float k)
{
	m5_power((enum thruster) id, k);
	m5_power_offer_resume();
}

void set_motors(float motors[NUM_MOTORS])
{
	for (int t = 0; t < NUM_MOTORS; t++) 
	{
		// Serial << "ID " << t+1 << " T " << motors[t] << '\n';
		// m5_power((enum thruster) motors[t].pos, motors[t].thrust);
		m5_power((enum thruster) t+1, limit(motors[t], -0.3, 0.3));
	}
	m5_power_offer_resume();
}

/*
 * TODO Still need to compute Y/P/R movement.
 */
void compute_motors(float dstate[DOF], float pid[DOF], float p)
{
	// Default motors to 0.
	float motors[NUM_MOTORS] = { 0.0f };

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
		for (int j = 0; j < DOF; j++) 
			motors[i] += p * pid[j] * ORIENTATION[i][j];
	set_motors(motors);
}

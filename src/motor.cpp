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
	m5_power(VERT_FL, motors[0]);
	m5_power(VERT_FR, motors[1]);
	m5_power(VERT_BL, motors[2]);
	m5_power(VERT_BR, motors[3]);
	m5_power(SURGE_FL, motors[4]);
	m5_power(SURGE_FR, motors[5]);
	m5_power(SURGE_BL, motors[6]);
	m5_power(SURGE_BR, motors[7]);
	m5_power_offer_resume();
}

void compute_motors(float dstate[DOF], float pid[DOF], float p)
{
	// Default motors to 0.
	float motors[NUM_MOTORS];
	for (int i = 0; i < NUM_MOTORS; i++)
		motors[i] = 0.0f;

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
		for (int j = 0; j < DOF; j++) 
			motors[i] += p * pid[j] * ORIENTATION[i][j];
	if (p > 0.01)
	{
		motors[0] += 0.15f;
		motors[1] -= 0.15f;
		motors[2] -= 0.15f;
		motors[3] += 0.15f;
	}
	set_motors(motors);
}

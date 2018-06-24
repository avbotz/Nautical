#include "streaming.h"
#include "pid.hpp"
#include "motor.hpp"
#include "util.hpp"


void powers(float motors[NUM_MOTORS])
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

uint32_t motors(PID controllers[DOF], float dstate[DOF], float mtr[NUM_MOTORS], float p, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp - t)/(float)(1000000);

	// Calculate PID values.
	float pid[DOF] = { 0.0f };
	for (int i = 0; i < DOF; i++)
		pid[i] = controllers[i].calculate(dstate[i], dt, 0.25);

	// Default motors to 0.
	for (int i = 0; i < NUM_MOTORS; i++)
		mtr[i] = 0.0f;

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
		for (int j = 0; j < DOF; j++) 
			mtr[i] += p * pid[j] * ORIENTATION[i][j];
	if (p > 0.01f)
	{
		mtr[0] += 0.15f;
		mtr[1] -= 0.15f;
		mtr[2] -= 0.15f;
		mtr[3] += 0.15f;
	}
	powers(mtr);

	return temp;
}

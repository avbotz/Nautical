#include "streaming.h"
#include "pid.hpp"
#include "motor.hpp"
#include "util.hpp"


void powers(float *motors)
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

uint32_t motors(PID *controllers, float *dstate, float *mtr, float p, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp - t)/(float)(1000000);

	// Calculate PID values.
	float pid[DOF] = { 0.0f };
	for (int i = 0; i < DOF; i++)
		pid[i] = controllers[i].calculate(dstate[i], dt, 0.25);

	/*
	// Default vert motors to 0.15 if power is on.
	for (int i = 0; i < 4; i++)
		mtr[i] = p < 0.01f ? 0.0f : 0.15f;
	*/

	// Default body motors to 0.
	for (int i = 0; i < 8; i++)
	{
		if (p < 0.01f)
			mtr[i] = 0.0f;
		else 
		{
			if (i == 0)
				mtr[i] = 0.15f;
			if (i == 1)
				mtr[i] = -0.15f;
			if (i == 2)
				mtr[i] = -0.15f;
			if (i == 3)
				mtr[i] = 0.15f;
			if (i > 3)
				mtr[i] = 0.0f;
		}
	}
	
	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
	{
		for (int j = 0; j < DOF; j++) 
		{
			float thrust = p * pid[j] * ORIENTATION[i][j];
			mtr[i] += p * pid[j] * ORIENTATION[i][j];
		}
	}
	powers(mtr);

	return temp;
}

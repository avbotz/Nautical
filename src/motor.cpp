#include "streaming.h"
#include "pid.hpp"
#include "motor.hpp"
#include "util.hpp"


Motors::Motors()
{
	for (int i = 0; i < NUM_MOTORS; i++)
		thrust[i] = 0.0f;
	for (int i = 0; i < DOF; i++)
		controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);
}

void Motors::power()
{
	m5_power(VERT_FL, thrust[0]);
	m5_power(VERT_FR, thrust[1]);
	m5_power(VERT_BL, thrust[2]);
	m5_power(VERT_BR, thrust[3]);
	m5_power(SURGE_FL, thrust[4]);
	m5_power(SURGE_FR, thrust[5]);
	m5_power(SURGE_BL, thrust[6]);
	m5_power(SURGE_BR, thrust[7]);
	m5_power_offer_resume();
}

uint32_t Motors::run(float *dstate, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp - t)/(float)(1000000);

	// Calculate PID values.
	float pid[DOF] = { 0.0f };
	for (int i = 0; i < DOF; i++)
		pid[i] = controllers[i].calculate(dstate[i], dt, 0.25);

	// Default body motors to 0.
	for (int i = 0; i < 8; i++)
	{
		if (p < 0.01f)
			thrust[i] = 0.0f;
		else 
		{
			if (i == 0)
				thrust[i] = 0.15f;
			if (i == 1)
				thrust[i] = -0.15f;
			if (i == 2)
				thrust[i] = -0.15f;
			if (i == 3)
				thrust[i] = 0.15f;
			if (i > 3)
				thrust[i] = 0.0f;
		}
	}
	
	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
	{
		for (int j = 0; j < DOF; j++) 
		{
			thrust[i] += p * pid[j] * ORIENTATION[i][j];
		}
	}
	power();

	return temp;
}

#include "streaming.h"
#include "motor.hpp"


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
		m5_power((enum thruster) t+1, motors[t]);
	}
	m5_power_offer_resume();
}

/*
 * TODO Still need to compute Y/P/R movement.
 */
void run_motors(const State	&current, const State &desired, float p)
{
	// Default motors to 0.
	float motors[NUM_MOTORS] = { 0.f };

	// Compute state difference.
	float dx = desired.x - current.x;
	float dy = desired.y - current.y;
	float dz = desired.z - current.z;

	// TODO Compute constants based on how far we are from the target.
	// Right now, these are set to + or - without regard to closeness.
	float kx = (dx < 0) ? -1 : 1;
	float ky = (dy < 0) ? -1 : 1;
	float kz = (dz < 0) ? -1 : 1;

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
	{
		motors[i] += kx * p * ORIENTATION[i][0]; 
		motors[i] += ky * p * ORIENTATION[i][1];
		motors[i] += kz * p * ORIENTATION[i][2];
	}

	// k-dir * d-dir returns a positive value.
	// 0.1 is to ensure that we are moving somewhere worthwhile.
	if (kx*dx > 0.1 || ky*dy > 0.1 || kz*dz > 0.1)
		set_motors(motors);
}

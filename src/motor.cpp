#include "streaming.h"
#include "pid.hpp"
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
void run_motors(const State	&current, const State &desired, PID controllers[DOF], float p, unsigned long start)
{
	// Default motors to 0.
	float motors[NUM_MOTORS] = { 0.0f };

	// Compute state difference.
	float dstate[DOF] = { 0.0f };
	dstate[0] = desired.x - current.x;
	dstate[1] = desired.y - current.y;
	dstate[2] = desired.z - current.z;
	
	// TODO Compute directional constants based on how far we are from the target.
	// Right now, these are set to + or - without regard to closeness.
	float dir[DOF] = { 1.0f };
	for (int i = 0; i < DOF; i++)
		dir[i] = (dstate[i] < 0.0f) ? -1.0f : 1.0f;
	
	// Calculate PID values using the state difference.
	float pid[DOF] = { 1.0f };
	float dt = (float)(micros() - start)/(float)(1000000);
	for (int i = 0; i < DOF; i++)
		pid[i] = controllers[i].calculate(dstate[i], dt);

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
		for (int j = 0; j < DOF; j++) 
			motors[i] += p * pid[j] * dir[j] * ORIENTATION[i][j];

	// k-dir * d-dir returns a positive value.
	// 0.1 is to ensure that we are moving somewhere worthwhile.
	if (k[0]*dstate[0] > 0.1f || k[1]*dstate[1] > 0.1f || k[2]*dstate[2] > 0.1f)
		set_motors(motors);
}

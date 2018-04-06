#include "motor.hpp"


Motor* init_motors()
{
	Motor motors[8];
	for (int i = 1; i < 8; i++)
	{
		motors[i-1] = { (enum thruster) i, 0.f };
	}

	return motors;
}

void set_powers(float vals[NUM_THRUSTERS])
{
	for (int t = NUM_THRUSTERS; t--;)
	{
		m5_power((enum thruster) t, vals[t]);
	}
}

void set_motor(Motor m)
{
	m5_power((enum thruster) m.pos, m.thrust);
	m5_power_offer_resume();
}

void set_motors(Motor *motors)
{
	for (int t = 0; t < NUM_THRUSTERS; t++) 
	{
		m5_power((enum thruster) motors[t].pos, motors[t].thrust);
	}
	m5_power_offer_resume();
}

/*
 * TODO Still need to compute Y/P/R movement.
 */
void run_motors(const State	&current, const State &desired, Motor *motors, float p)
{
	// Compute state difference 
	int dx = desired.x - current.x;
	int dy = desired.y - current.y;
	int dz = desired.z - current.z;

	// TODO Compute constants based on how far we are from the target
	// Right now, these are set to + or - without regard to closeness
	int kx = (dx < 0) ? -1 : 1;
	int ky = (dy < 0) ? -1 : 1;
	int kz = (dz < 0) ? -1 : 1;

	// Compute final thrust given to each motor based on orientation matrix
	for (int i = 0; i < 8; i++)
	{
		motors[i].thrust += kx * p;
		motors[i].thrust += ky * p;
		motors[i].thrust += kz * p;
	}
}

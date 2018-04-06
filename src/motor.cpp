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
	for (int t = 1; t <= NUM_THRUSTERS; t++) 
	{
		m5_power((enum thruster) motors[t].pos, motors[t].pos);
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
	int kx = 1;
	int ky = 1;
	int kz = 1;

}

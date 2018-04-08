#include "streaming.h"
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

void set_motors(float motors[NUM_THRUSTERS])
{
	for (int t = 0; t < NUM_THRUSTERS; t++) 
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
void run_motors(const State	&current, const State &desired, float motors[NUM_THRUSTERS], float p)
{
	// Reset motors to 0
	for (int i = 0; i < 8; i++) 
	{
		motors[i] = 0.f;
	}

	// Compute state difference 
	float dx = desired.x - current.x;
	float dy = desired.y - current.y;
	float dz = desired.z - current.z;
	// Serial << dx << " " << dy << " " << dz << '\n';
	// float dx=1, dy=0, dz=0;

	// TODO Compute constants based on how far we are from the target
	// Right now, these are set to + or - without regard to closeness
	float kx = (dx < 0) ? -1 : 1;
	float ky = (dy < 0) ? -1 : 1;
	float kz = (dz < 0) ? -1 : 1;

	// Compute final thrust given to each motor based on orientation matrix
	for (int i = 0; i < 8; i++)
	{
		motors[i] += kx * p; 
		motors[i] += ky * p;
		motors[i] += kz * p;
	}

	if (kx*dx > 0.1 || ky*dy > 0.1 || kz*dz > 0.1)
	{
		// Serial << "Setting thrust!" << '\n';
		set_motors(motors);
	}
}

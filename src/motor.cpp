#include "streaming.h"
#include "pid.hpp"
#include "motor.hpp"
#include "m5/io_m5.h"
#include "util.hpp"
#include "rotation.h"


Motors::Motors()
{
	for (int i = 0; i < DOF; i++)
		this->controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);
	for (int i = 0; i < NUM_MOTORS; i++)
		this->thrust[i] = 0.0f;
	for (int i = 0; i < NUM_MOTORS; i++)
		this->buttons[i] = 0;
	for (int i = 0; i < DOF; i++)
		this->forces[i] = 0.0f;
	for (int i = 0; i < DOF; i++)
		this->pid[i] = 0.0f;
	this->p = 0.0f;
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

void Motors::pause()
{
	io_m5_trans_stop();
}

void Motors::wiimote(int *buttons)
{
	for (int i = 0; i < NUM_MOTORS; i++)
		thrust[i] = 0.0f;

	// Default down thrust, used to keep sub in place.
	thrust[0] += 0.15;
	thrust[1] -= 0.15;
	thrust[2] -= 0.15;
	thrust[3] += 0.15;

	// Up button.
	if (buttons[0])
	{
		thrust[4] += 0.15;
		thrust[5] -= 0.15;
		thrust[6] -= 0.15;
		thrust[7] += 0.15;
	}

	// Down button.
	if (buttons[1])
	{
		thrust[4] -= 0.15;
		thrust[5] += 0.15;
		thrust[6] += 0.15;
		thrust[7] -= 0.15;
	}

	// Left button.
	if (buttons[2])
	{
		thrust[4] -= 0.15;
		thrust[5] -= 0.15;
		thrust[6] -= 0.15;
		thrust[7] -= 0.15;
	}

	// Right button.
	if (buttons[3])
	{
		thrust[4] += 0.15;
		thrust[5] += 0.15; 
		thrust[6] += 0.15;
		thrust[7] += 0.15;
	}

	// Left yaw button (1).
	if (buttons[4])
	{
		thrust[4] -= 0.15;
		thrust[5] -= 0.15;
		thrust[6] += 0.15;
		thrust[7] += 0.15;
	}

	// Right yaw button (2).
	if (buttons[5])
	{
		thrust[4] += 0.15;
		thrust[5] += 0.15;
		thrust[6] -= 0.15;
		thrust[7] -= 0.15;
	}

	// Down button (B).
	if (buttons[6]) 
	{
		thrust[0] += 0.15;
		thrust[1] -= 0.15;
		thrust[2] -= 0.15;
		thrust[3] += 0.15;
	}

	// Up button (B).
	if (buttons[7]) 
	{
		thrust[0] -= 0.15;
		thrust[1] += 0.15;
		thrust[2] += 0.15;
		thrust[3] -= 0.15;
	}

	// Power the motors.
	// for (int i = 0; i < 10; i++)
	//    if (!SIM) power();
}

uint32_t Motors::run(float *dstate, float *angles, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp - t)/(float)(1000000);

	// Calculate PID values.
	pid[F] = controllers[F].calculate(dstate[F], dt, 0.20);
	pid[H] = controllers[H].calculate(dstate[H], dt, 0.20);
	pid[V] = controllers[V].calculate(dstate[V], dt, 0.00);
	for (int i = BODY_DOF; i < GYRO_DOF; i++)
		pid[i] = controllers[i].calculate(dstate[i], dt, 0.0);

	// Default body motors to 0.
	for (int i = 0; i < NUM_MOTORS; i++)
		thrust[i] = 0.0f;
	if (p > 0.01f)
	{
		thrust[0] += 0.15f;
		thrust[1] -= 0.15f;
		thrust[2] -= 0.15f;
		thrust[3] += 0.15f;
	}

	// Compute final thrust given to each motor based on orientation matrix.
	for (int i = 0; i < NUM_MOTORS; i++)
		for (int j = 0; j < DOF; j++) 
			thrust[i] += p * pid[j] * ORIENTATION[i][j];
	if (!SIM) power();

	// Compute forces from motors.
	float bforces[BODY_DOF];
	float MOUNT_ANGLE = sin(45.0);
	for (int i = 0; i < BODY_DOF; i++)
		bforces[i] = 0.0f;
	bforces[F] = MOUNT_ANGLE*(thrust[4]-thrust[5]-thrust[6]+thrust[7]);
	bforces[H] = MOUNT_ANGLE*(thrust[4]+thrust[5]+thrust[6]+thrust[7]);
	bforces[V] = thrust[0]-thrust[1]-thrust[2]+thrust[3];
	float iforces[BODY_DOF];
	body_to_inertial(bforces, angles, iforces);
	forces[F] = iforces[F];
	forces[H] = iforces[H];
	forces[V] = iforces[V]; 

	return temp;
}

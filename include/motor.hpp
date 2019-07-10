#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "pid.hpp"

#define PAUSE_TIME 4500 

struct Motors
{
	PID controllers[DOF];
	int buttons[NUM_MOTORS];
	float thrust[NUM_MOTORS];
	float forces[DOF];
	float pid[DOF];
	float p;

	Motors();

	void power();
	void pause();
	uint32_t run(float *dstate, float *angles, uint32_t t);
};

#endif 

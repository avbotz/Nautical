#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "pid.hpp"

struct Motors
{
	PID controllers[DOF];
	float thrust[NUM_MOTORS];
	float pid[DOF];
	float p;
	Motors();

	void power();
	uint32_t run(float *dstate, uint32_t t);
};

#endif 

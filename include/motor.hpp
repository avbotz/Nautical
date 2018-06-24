#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "pid.hpp"

void powers(float motors[NUM_THRUSTERS]);
uint32_t motors(PID controllers[DOF], float dstate[DOF], float mtr[NUM_MOTORS], float p, uint32_t t);

#endif 

#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "state.hpp"
#include "pid.hpp"

void set_motor(int id, float p);
void set_motors(float motors[NUM_THRUSTERS]);
void compute_motors(float dstate[DOF], float pid[DOF], float p);

#endif 

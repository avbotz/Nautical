
#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "state.hpp"
#include "pid.hpp"

void set_motor(int id, float p);
void set_motors(float motors[NUM_THRUSTERS]);
uint32_t run_motors(const State &current, const State &desired, PID controllers[DOF], float p, uint32_t start);

#endif 

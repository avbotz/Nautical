#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "state.hpp"

void set_motor(int id, float p);
void set_motors(float motors[NUM_THRUSTERS]);
void run_motors(const State &current, const State &desired, float p);

#endif 

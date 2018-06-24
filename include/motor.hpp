#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "pid.hpp"

void powers(float *motors);
uint32_t motors(PID *controllers, float *dstate, float *mtr, float p, uint32_t t);

#endif 

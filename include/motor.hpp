#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "state.hpp"

struct Motor 
{
	enum thruster pos;
	float thrust;
};

struct Motor* init_motors();
void set_powers(float vals[NUM_THRUSTERS]);
void set_motor(struct Motor);
void set_motors(struct Motor*);
void run_motors(const State&, const State&, struct Motor*);

#endif 

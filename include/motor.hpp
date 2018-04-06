#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "state.hpp"

/*
 * Motor Orientation Configuration.
 * Rows = motors, Columns = directions, Values = weights.
 */
static const int ORIENTATION[8][6] = 
{
	//X, 	Y, 	 Z, 	Yaw,  Pitch Roll
	{ 0,	0,	 -1,	0,	  1,	-1 },
	{ 0,	0,	 1,		0, 	  -1,   -1 },
	{ 0,	0,	 1,		0, 	  1, 	1 },
	{ 0,	0,	 -1,	0, 	  -1, 	1 },
	{ -1,	1,	 0,		1, 	  0, 	0 },
	{ 1,	1,	 0,		1, 	  0,	0 },
	{ 1,	1,	 0,		-1,   0,	0 }, 
	{ -1,	1,	 0,		-1,   0,	0 }
};

struct Motor 
{
	enum thruster pos;
	float thrust;
};

struct Motor* init_motors();
void set_motors();
// void run_motors(const State&, const State&, struct Motor*);

#endif 

#ifndef MOTOR_HPP
#define MOTOR_HPP 

/*
 * Motor ID Configuration.
 * 8 motors on the sub.
 * VERTICAL + SURGE for FL, FR, BL, BR.
 */
enum thruster
{
	VERT_FL = 1,
	VERT_FR = 2,
	VERT_BL = 3,
	VERT_BR = 4,
	SURGE_FL = 5,
	SURGE_FR = 6,
	SURGE_BL = 7,
	SURGE_BR = 8,
	NUM_THRUSTERS
};

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

Motor* init_motors();
void set_motors();
// void run_motors(const State&, const State&, Motor*);

#endif 

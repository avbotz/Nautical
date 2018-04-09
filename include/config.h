#ifndef CONFIG_H
#define CONFIG_H 

/*
 * Motor ID Configuration.
 * 8 motors on the sub.
 * VERTICAL + SURGE for FL, FR, BL, BR.
 * Important! These must be in order from 1-8.
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
static const int NUM_MOTORS = 8;

/*
 * Motor Orientation Configuration.
 * Rows = motors, Columns = directions, Values = weights.
 * 1 means clockwise, -1 means counterclockwise, 0 means not used.
 * TODO Someone needs to double check this orientation matrix!
 */
static const int ORIENTATION[8][6] = 
{
	//X, 	Y, 	 Z, 	Y,	  P, 	R
	{ 0,	0,	 -1,	0,	  1,	-1 },
	{ 0,	0,	 1,		0, 	  -1,   -1 },
	{ 0,	0,	 1,		0, 	  1, 	1 },
	{ 0,	0,	 -1,	0, 	  -1, 	1 },
	{ -1,	1,	 0,		1, 	  0, 	0 },
	{ 1,	1,	 0,		1, 	  0,	0 },
	{ 1,	1,	 0,		-1,   0,	0 }, 
	{ -1,	1,	 0,		-1,   0,	0 }
};

#endif 

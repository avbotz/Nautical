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
 */
static const int ORIENTATION[8][6] = 
{
	{ 0, 0,	1, 0, 1, -1 },
	{ 0, 0, -1, 0, -1, -1 },
	{ 0, 0, -1,	0,  1, 1 },
	{ 0, 0,	1, 0, -1, 1 },
	{ 1, 1, 0, 1, 0, 0 },
	{ -1, 1, 0,	1, 0, 0 },
	{ -1, 1, 0,	-1, 0, 0 }, 
	{ 1, 1, 0, -1, 0, 0 }
};

/*
 * PID Gains Configuration.
 * Rows correspond to X, Y, Z, Y, P, R. Cols correspond to kp, ki, kd.
 */
static const int DOF = 6;
static const int MOVE_DOF = 3;
static const float GAINS[6][3] = 
{
	{ 2.00, 0, 0 },
	{ 1.25, 0, 0 },
	{ 1.50, 0, 0 },
	{ 0.10, 0, 0 },
	{ 0.10, 0, 0 },
	{ 0.10, 0, 0 }
};

#endif 

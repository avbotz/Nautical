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
	{ 1,	1,	 0,		1, 	  0, 	0 },
	{ -1,	1,	 0,		1, 	  0,	0 },
	{ -1,	1,	 0,		-1,   0,	0 }, 
	{ 1,	1,	 0,		-1,   0,	0 }
};

/*
 * PID Gains Configuration.
 * Rows correspond to X, Y, Z, Y, P, R. Cols correspond to kp, ki, kd.
 */
static const float GAINS[6][3] = 
{
 	{ 0.75, 0, 0 },
	{ 0.75, 0, 0 },
	{ 0.75, 0, 0 },
	{ 0.05, 0, 0 },
	{ 0.05, 0, 0 },
	{ 0.05, 0, 0 }
};

/*
 * State configuration.
 * This should never change, unless our state system changes or we gain another
 * degree of freedom (never).
 * Also GYRO_DOF refers to axes 4-6.
 */
static const int DOF = 6;
static const int MOVE_DOF = 3;
static const int GYRO_DOF = 6;

static const int X 			= 0;
static const int Y 			= 1;
static const int Z 			= 2;
static const int S_YAW 		= 3;
static const int S_PITCH 	= 4;
static const int S_ROLL 	= 5;
static const int S_SURGE 	= 0;
static const int S_SWAY 	= 1;
static const int S_HEAVE 	= 2;

#endif 

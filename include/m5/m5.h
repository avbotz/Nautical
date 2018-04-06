#ifndef M5_H
#define M5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/*
 * Motor ID Configuration.
 * 8 motors on the sub.
 * VERTICAL + SURGE for FL, FR, BL, BR.
 * FYI: I would have preferred to put this in motor.hpp, but it needs to be in a
 * .h file. 
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

/**
 * writes the next byte of the propulsion command packet to the m5's
 *
 * returns EOF on failure writing byte, 1 on packet completion, and zero on non
 * packet completion.
 */
int m5_power_trans();

/**
 * Sets thruster t to be set to power.
 * 
 * power must range [-1, 1]
 *
 * The new value will not actually be transmitted until m5_power_offer_resume
 * is called.
 */
void m5_power(enum thruster t, float power);

/**
 * Sets the most recent power value set for each thruster with m5_power to be
 * transmitted as soon as the next set of thrust values begin transmission.
 *
 * Resumes transmission if it is currently paused.
 */
void m5_power_offer_resume();

#ifdef __cplusplus
}
#endif

#endif

#ifndef M5_H
#define M5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "motor.hpp"

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

/** @file motor.hpp
 *  @brief Helper class to use all of the motors at once and do computations.
 *
 *  @author David Zhang
 */
#ifndef MOTOR_HPP
#define MOTOR_HPP 

#include "m5/m5.h"
#include "config.h"
#include "pid.hpp"

/** Startup time for motors after sub is unkilled.
 */
#define PAUSE_TIME 4500 

/** @brief Helper class for motors.
 */
struct Motors
{
	/** PID controllers for each degree of freedom. */
	PID controllers[DOF];

	/** Current thrust values for each of the motors. */
	float thrust[NUM_MOTORS];

	/** Holds pressed values for remote control. */
	int buttons[NUM_MOTORS];

	/** Theoretical forces for each degree of freedom. */
	float forces[DOF];

	/** Computed PID values from each controller. */
	float pid[DOF];

	/** Current submarine power. */
	float p;

	Motors();

	/** @brief Set power to the motors using the current thrust vector. 
	 */
	void power();

	/** @brief Pause power to the motors. 
	 */
	void pause();

	/** @brief Run the motors for one iteration towards the desired state.
	 *
	 *  @param dstate Difference between desired and current state.
	 *  @param angles Current euler angles.
	 *  @param t Current time used for time difference calculations.
	 *  @return Time after iteration is finished.
	 */
	uint32_t run(float *dstate, float *angles, uint32_t t);
};

#endif 

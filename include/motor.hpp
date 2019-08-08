/*
 * MIT License
 * 
 * Copyright (c) 2019 AVBotz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ========================================================================== */

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
	PID controllers[DOF+1];

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
	 *  @param daltitude Difference between distances from bottom. 
	 *  @param angles Current euler angles.
	 *  @param t Current time used for time difference calculations.
	 *  @return Time after iteration is finished.
	 */
	uint32_t run(float *dstate, float daltitude, float *angles, uint32_t t);
};

#endif 

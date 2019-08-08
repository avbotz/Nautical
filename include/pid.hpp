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

/** @file pid.hpp
 *  @brief Helper class to compute PID for motors.
 *
 *  @author David Zhang
 */
#ifndef PID_HPP
#define PID_HPP 

/** @brief Helper class for PID computations.
 */
struct PID 
{
	/** PID gains for proportional, integral, and derivative. */
	float kp, ki, kd;

	/** Previous error value. */
	float prev;
	
	/** Sum of all of the errors. */
	float sum;

	PID() {}
	PID(float a, float b, float c) : kp(a), ki(b), kd(c), prev(0.), sum(0.) {}

	/** @brief Initialize the PID gains from the config.
	 *  
	 *  @param a Proportional gain.
	 *  @param b Integral gain.
	 *  @param c Derivative gain.
	 *  @return 0 on success.
	 */
	float init(float a, float b, float c);

	/** @brief Compute total PID constant.
	 *  
	 *  @param error Difference between setpoint and current point.
	 *  @param dt Time difference.
	 *  @param min Minimum value of PID, which is usefull to ensure small changes 
	 *             are adjusted for.
	 *  @return The total PID constant.
	 */
	float calculate(float error, float dt, float min);
};

#endif 

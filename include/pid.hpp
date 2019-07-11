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

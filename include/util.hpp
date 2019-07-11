/** @file util.hpp
 *  @brief Helper functions for other Nautical functions.
 *
 *  @author David Zhang
 */
#ifndef UTIL_HPP
#define UTIL_HPP

/** @brief Difference between two angles.
 *
 *  For example, if a1 is -170 and a2 is 170, a1-a2 = -340, which isn't how much
 *  the true angle difference is. Instead, it should be -20. This functions 
 *  covers the edge cases.
 *
 *  @param a1 Desired angle.
 *  @param a2 Current angle.
 *  @return Angle difference.
 */
float angle_difference(float a1, float a2);

/** @brief Sum of two angles.
 *
 *  For example, if a1 is 50 and a2 is 150, a1+a2 = 200, which isn't how much
 *  the true angle difference is. Instead, it should be -160. This functions 
 *  covers the edge cases.
 *
 *  @param a1 Desired angle.
 *  @param a2 Current angle.
 *  @return Angle difference.
 */
float angle_add(float a1, float add);

/** @brief Limits input between two numbers.
 *
 *  This functions is used for PID.
 *
 *  @param lower Lower bound.
 *  @param upper Upper bound.
 *  @return Input after limiting.
 */
float limit(float input, float lower, float upper);

/** @brief Limits input to be at least a certain number.
 *
 *  This functions is used for PID.
 *
 *  @param min Lower bound.
 *  @return Input after limiting.
 */
float limit(float input, float min);

#endif

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

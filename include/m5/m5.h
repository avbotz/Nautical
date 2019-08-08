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

/** @file m5.h
 *  @brief Interface function definitions for Videoray M5 motors.
 *  
 *  @author Seth Girvan (Lord)
 */
#ifndef M5_H
#define M5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "config.h"

/** @brief Writes the next byte of the propulsion command packet to the M5s.
 *
 *  @return EOF on failure writing byte, 1 on packet completion, and zero on 
 *          non-packet completion.
 */
int m5_power_trans();

/** @brief Sets thruster t to "power" power.
 * 
 *  The new value will not actually be transmitted until m5_power_offer_resume
 *  is called.
 *
 *  @param t The thruster that is being powered.
 *  @param power The amount of power, in range of [-1, 1].
 */
void m5_power(enum thruster t, float power);

/** @brief Gives power to all the motors. 
 *
 *  Sets the most recent power value set for each thruster with m5_power to be
 *  transmitted as soon as the next set of thrust values begin transmission.
 *
 *  Resumes transmission if it is currently paused.
 */
void m5_power_offer_resume();

#ifdef __cplusplus
}
#endif

#endif

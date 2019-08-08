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

/** @file io.hpp
 *  @brief IO and kill switch function definitions.
 *
 *  @author David Zhang
 */
#ifndef IO_HPP
#define IO_HPP

#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"
#include "io.hpp"
#include "servo.h"

#define DEPTH_PIN A0
#define KILL_PIN 30

extern ServoTimer2 dropper1;
extern ServoTimer2 dropper2;

/** @brief Starts IO initialization for all major hardware (AHRS, DVL, M5, depth
 *         sensor, kill switch).
 */
void io();

/** @brief Turns servo to drop ball from dropper.
 *  
 *  @param idx 0 drops first servo, 1 drops second.
 *  @param val 0 means in, 1 means out.
 */
void drop(int idx, int val);

/** @brief Reads kill state from the kill switch.
 *  
 *  @return True on alive.
 */
bool alive();

#endif

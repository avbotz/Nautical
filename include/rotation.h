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

/** @file rotation.h
 *  @brief Converts between frames of reference.
 *
 *  @author David Zhang
 */
#ifndef ROTATION_H
#define ROTATION_H

#include <Arduino.h> 

/** @brief Converts values from body frame of reference to inertial frame of
 *         reference.
 *  
 *  @param input Pointer to where the values of the body frame are.
 *  @param angles Current euler angles.
 *  @param output Pointer to where the values in the inertial frame will be 
 *                stored.
 */
void body_to_inertial(float *input, float *angles, float *output);

#endif 

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

/** @file voltage.hpp
 *  @brief Calculates remaining voltage on batteries.
 *
 *  @author Maxim Vovenko
 */
#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

#define NUM_SAMPLES 10

/** @brief Calculate voltage left.
 *
 *  I'm not sure how well this function works, it is untested at the moment. It
 *  needs to be calibrated for sure.
 *
 *  @return Remaining voltage.
 */
float voltage();

#endif

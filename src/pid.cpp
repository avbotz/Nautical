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

#include "pid.hpp"
#include "util.hpp"


float PID::init(float a, float b, float c)
{
	this->kp = a;
	this->ki = b;
	this->kd = c;
	this->prev = 0.0;
	this->sum = 0.0;
}

/*
 * IMPORTANT: Make sure DT isn't 0, will cause motors to send NAN.
 */
float PID::calculate(float error, float dt, float min)
{
	float pout = this->kp*error;
	this->sum += error*dt;
	float iout = this->ki*this->sum;
	float dout = this->kd*(error-this->prev)/dt;
	this->prev = error;
	float output = pout + iout + dout;
	float dir = 1.;
	if (output < 0) dir = -1.;

	float out = limit(limit(output, -2., 2.), min);
	return out;
}

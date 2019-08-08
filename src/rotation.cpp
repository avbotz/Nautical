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

#include <Arduino.h>
#include "config.h"
#include "rotation.h"


void body_to_inertial(float *input, float *angles, float *output)
{
	// Pre-compute trig functions.
	float spsi = sin(angles[0]*D2R);
	float sthe = sin(angles[1]*D2R);
	float sphi = sin(angles[2]*D2R);
	float cpsi = cos(angles[0]*D2R);
	float cthe = cos(angles[1]*D2R);
	float cphi = cos(angles[2]*D2R);

	// Calculate rotation matrix for Euler transformation.
	float r11 =  cthe*cpsi;
	float r12 = -cphi*spsi + sphi*sthe*cpsi;
	float r13 = -sphi*spsi + cphi*sthe*cpsi;
	float r21 =  cthe*spsi;
	float r22 =  cphi*cpsi + sphi*sthe*spsi;
	float r23 =  sphi*cpsi + cphi*sthe*spsi;
	float r31 = -sthe;
	float r32 =  sphi*cthe;
	float r33 =  cphi*cthe;

	// Calculate matrix.
	output[0] = r11*input[0] + r12*input[1] + r13*input[2];
	output[1] = r21*input[0] + r22*input[1] + r23*input[2];
	output[2] = r31*input[0] + r32*input[1] + r33*input[2];
}

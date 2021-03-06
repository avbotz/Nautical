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
#include "ahrs/ahrs.h"
#include "dvl/dvl.h"
#include "streaming.h"
#include "config.h"
#include "matrix.h"
#include "kalman.hpp"
#include "rotation.h"


Kalman::Kalman()
{
	this->skip = 1000;
	this->iter = 1000;
	for (int i = 0; i < M; i++)
	{
		m_orig[i] = 0.;
		m_bias[i] = 0.;
	}
}

void Kalman::bias()
{
	// Compute initial bias from the accelerometer.
	for (int i = 0; i < M; i++)
		m_bias[i] = 0.;
	for (int i = 0; i < skip; i++)
	{
		delay(1);
		ahrs_att_update();
	}
	for (int i = 0; i < iter; i++)
	{
		delay(1);
		ahrs_att_update();
		m_bias[0] += ahrs_accel((enum accel_axis)(SURGE));
		m_bias[1] += ahrs_accel((enum accel_axis)(SWAY));
	}
	m_bias[0] /= (float)iter;
	m_bias[1] /= (float)iter;
}

uint32_t Kalman::compute(float *state, float *covar, float *angles, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp-t)/1000000.;

	// Convert DVL velocities from um/s to m/s.
	float *m = new float[3];
	float t1 = dvl_get_forward_vel()/100000.;
	float t2 = dvl_get_starboard_vel()/100000.;

	// DVL is oriented at a 45 degree offset from what we want. U and V
	// represent the velocities with respect to the body orientation.
	float u = cos(45.*D2R)*t1 - sin(45.*D2R)*t2;
	float v = sin(45.*D2R)*t1 + cos(45.*D2R)*t2;

	// Check if DVL has returned error velocity.
	m_orig[0] = dvl_get_forward_vel();
	m_orig[1] = dvl_get_starboard_vel();
	if (fabs(t1) > 32. || fabs(t2) > 32.)
	{
		// Serial << "DVL did not return data." << _FLOAT(t1, 6) << " " << 
		//	_FLOAT(t2, 6) << endl;
		delete[] m;
		// delete[] Kk;
		return temp;
	}

	// Convert from body to inertial reference frame and multiply by time
	// difference to get change in distance.
	float temparr[3] = {u, v, 0};
	body_to_inertial(temparr, angles, m);
	m_orig[0] = m[0];
	m_orig[1] = m[1];
	state[0] += m[0]*dt;
	state[3] += m[1]*dt;
	delete[] m; 

	/*
	 * The Kalman filter is turned off until a better sub movement model can be
	 * found. This would require manual calibration for power/acceleration
	 * ratios and a lot of testing. 
	 *
	 * It might be a good idea to Kalman filter depth or attitude data, but
	 * these seem to be accurate for now.
	 */
	/*
	// Predict new state using model.
	// X, VX, AX, Y, VY, AY.
	float *a1 = new float[N];
	float Fk[N*N] = {
	1, dt, 0, 0, 0, 0,
	0, 1, dt, 0, 0, 0,
	0, 0, 1, 0, 0, 0,
	0, 0, 0, 1, dt, 0,
	0, 0, 0, 0, 1, dt,
	0, 0, 0, 0, 0, 1
	};
	multiply(Fk, state, N, N, 1, a1);
	memcpy(state, a1, sizeof(float)*N);
	delete[] a1;

	// Predict new covariance.
	float *b1 = new float[N*N];
	float *b2 = new float[N*N];
	float *b3 = new float[N*N];
	multiply(Fk, covar, N, N, N, b1);
	transpose(Fk, N, N, b2);
	multiply(b1, b2, N, N, N, b3);
	add(Qk, b3, N, N, covar);	
	delete[] b1;
	delete[] b2;
	delete[] b3;

	// Calculate the Kalman gain.
	float *Kk = new float[N*M];
	float *c1 = new float[N*M];
	float *c2 = new float[N*M];
	float *c3 = new float[M*N];
	float *c4 = new float[M*M];
	float *c5 = new float[M*M];
	transpose(Hk, M, N, c1);
	multiply(covar, c1, N, N, M, c2);
	multiply(Hk, covar, M, N, N, c3);
	multiply(c3, c1, M, N, M, c4);
	add(Rk, c4, M, M, c5);
	invert(c5, M);
	multiply(c2, c5, N, M, M, Kk);
	delete[] c1;
	delete[] c2;
	delete[] c3;
	delete[] c4;
	delete[] c5;

	// Receive measurements, taking into account accelerometer bias. Harsh
	// cutoff to reduce accelerometer drift.
	float *m = new float[M];
	float t1 = dvl_get_forward_vel()/100000.0f;
	float t2 = dvl_get_starboard_vel()/100000.0f;
	float u = cos(45.f*D2R)*t1 - sin(45.f*D2R)*t2;
	float v = sin(45.f*D2R)*t1 + cos(45.f*D2R)*t2;
	m_orig[0] = dvl_get_forward_vel();
	m_orig[1] = dvl_get_starboard_vel();
	if (fabs(t1) > 3.0f || fabs(t2) > 3.0f)
	{
	delete[] m;
	// delete[] Kk;
	return temp;
	}
	m[0] = u*cos(heading*D2R) - v*sin(heading*D2R);
	m[1] = u*sin(heading*D2R) + v*cos(heading*D2R);
	m_orig[0] = m[0];
	m_orig[1] = m[1];
	state[0] += m[0]*dt;
	state[3] += m[1]*dt;
	delete[] m; 

	// Update state using measurements and Kalman gain.
	float *d1 = new float[M];
	float *d2 = new float[M];
	float *d3 = new float[N];
	float *d4 = new float[N];
	multiply(Hk, state, M, N, 1, d1);
	subtract(m, d1, M, 1, d2);
	multiply(Kk, d2, N, M, 1, d3);
	add(d3, state, N, 1, d4);
	memcpy(state, d4, sizeof(float)*N);
	delete[] d1;
	delete[] d2;
	delete[] d3;
	delete[] d4;

	// Update error covariance using Kalman gain.
	float *e1 = new float[M*N];
	float *e2 = new float[N*N];
	float *e3 = new float[N*N];
	multiply(Hk, covar, M, N, N, e1);
	multiply(Kk, e1, N, M, N, e2);
	subtract(covar, e2, N, N, e3);
	memcpy(covar, e3, sizeof(float)*N*N);
	delete[] e1;
	delete[] e2;
	delete[] e3;

	delete[] m;
	delete[] Kk;
	*/
	return temp;
}


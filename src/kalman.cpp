#include <Arduino.h>
#include "ahrs/ahrs.h"
#include "config.h"
#include "matrix.h"
#include "kalman.hpp"


void Kalman::bias()
{
	// Compute initial bias from the accelerometer.
	afbias=0.0, ahbias=0.0;
	for (int i = 0; i < 100; i++)
	{
		delay(1);
		ahrs_att_update();
	}
	for (int i = 0; i < 100; i++)
	{
		delay(1);
		ahrs_att_update();
		afbias += ahrs_accel((enum accel_axis)(SURGE));
		ahbias += ahrs_accel((enum accel_axis)(SWAY));
	}
	afbias /= 100.0;
	ahbias /= 100.0;

}

uint32_t Kalman::compute(float *state, float *covar, uint32_t t)
{
	// Calculate time difference since last iteration.
	uint32_t temp = micros();
	float dt = (temp - t)/(float)(1000000);

	// Predict new state using model.
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
	m[0] = ahrs_accel((enum accel_axis)(SURGE)) - afbias;
	m[1] = ahrs_accel((enum accel_axis)(SWAY)) - ahbias;
	m[0] = m[0] < 0.05 ? 0.0 : m[0];
	m[1] = m[1] < 0.05 ? 0.0 : m[1];

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

	return temp;
}


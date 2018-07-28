#ifndef KALMAN_HPP
#define KALMAN_HPP

#include "motor.hpp"

/*
 * N represents the number of elements in the state, while M represents the
 * number of sensors.
 */
static const int N = 6;
static const int M = 2;

/*
 * Qk describes how accurate the model is.
 */
static float Qk[N*N] = {
	0.005, 0.000, 0.000, 0.000, 0.000, 0.000,
	0.000, 0.005, 0.000, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.005, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.005, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.005, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.000, 0.005
};

/*
 * Hk maps the predicted state to measurements.
 */
static float Hk[M*N] = {
 	0.000, 1.000, 0.000, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.000, 1.000, 0.000
};

/*
 * Rk describes the accuracy of each sensor.
 */
static float Rk[M*M] = {
	5.000, 0.000,
	0.000, 5.000
};

struct Kalman
{
	int skip, iter;
	float m_orig[M];
	float m_bias[M];
	Kalman();

	void bias();
	uint32_t compute(const Motors &motors, float *state, float *covar, uint32_t t);
};

#endif 

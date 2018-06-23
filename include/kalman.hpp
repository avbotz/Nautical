#ifndef KALMAN_HPP
#define KALMAN_HPP

/*
 * Initial bias assumes that sub starts at 0 accleration. This means that all
 * accelerations are relative to the initial state.
 */
static float afbias;
static float ahbias;

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
 	0.000, 0.000, 1.000, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.000, 1.000
};

/*
 * Rk describes the accuracy of each sensor.
 */
static float Rk[M*M] = {
	5.000, 0.000,
	0.000, 5.000
};

uint32_t kalman(float *state, float *covar, uint32_t t);

#endif 

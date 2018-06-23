#ifndef KALMAN_HPP
#define KALMAN_HPP

static float afbias;
static float ahbias;

static const int N = 6;
static const int M = 2;

static const float Qk[N*N] = {
	0.001, 0.000, 0.000, 0.000, 0.000, 0.000,
	0.000, 0.001, 0.000, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.001, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.001, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.001, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.000, 0.001
};

static const float Hk[M*N] = {
 	0.000, 0.000, 1.000, 0.000, 0.000, 0.000,
	0.000, 0.000, 0.000, 0.000, 0.000, 1.000
};

static const float Rk[M*M] = {
	5.000, 0.000,
	0.000, 5.000
};

uint32_t kalman(float *state, float *covar, uint32_t t);

#endif 

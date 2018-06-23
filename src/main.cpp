#include <Arduino.h>
#include "ahrs/ahrs.h"
#include "streaming.h"
#include "config.h"
#include "kalman.hpp"
#include "motor.hpp"
#include "util.hpp"
#include "pid.hpp"
#include "io.hpp"


void run()
{
	// Start IO between Nautical and hardware.
	io();

	// Initial motor strength at 0.
	float p = 0.0f;

	// Current state represents location, while desired state holds destination.
	float *current = new float[DOF];
	float *desired = new float[DOF];

	// Kalman filter initial state and error covariance.
	float state[N] = {
		0.000, 0.000, 0.000, 0.000, 0.000, 0.000
	};
	float covar[N*N] = {	
		1.000, 0.000, 0.000, 0.000, 0.000, 0.000,
		0.000, 1.000, 0.000, 0.000, 0.000, 0.000,
		0.000, 0.000, 1.000, 0.000, 0.000, 0.000,
		0.000, 0.000, 0.000, 1.000, 0.000, 0.000,
		0.000, 0.000, 0.000, 0.000, 1.000, 0.000,
		0.000, 0.000, 0.000, 0.000, 0.000, 1.000,
	};
	
	// Compute initial bias from the accelerometer.
	for (int i = 0; i < 100; i++)
		ahrs_att_update();
	for (int i = 0; i < 100; i++)
	{
		ahrs_att_update();
		afbias += ahrs_accel((enum accel_axis)(SURGE));
		ahbias += ahrs_accel((enum accel_axis)(SWAY));
	}
	afbias /= 100.0;
	ahbias /= 100.0;

	// Initial time, helps compute time difference.
	uint32_t ktime = micros();

	while (true)
	{
		// Important, otherwise data is sent too fast to m5.c.
		delay(10);

		// Kalman filter removes noise from measurements and estimates the new
		// state (linear).
		ktime = kalman(state, covar, ktime);
	}
}

void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

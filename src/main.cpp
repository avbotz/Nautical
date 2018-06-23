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

	// Initial time, helps compute time difference.
	uint32_t ktime = micros();

	while (true)
	{
		// Important, otherwise data is sent too fast.
		delay(10);
		ahrs_att_update();

		// Check for user input.
		if (Serial.available() > 0)
		{
			char c = Serial.read();

			// Return accelerometer data. 
			if (c == 'b')
				Serial << _FLOAT(afbias, 6) << ' ' << _FLOAT(ahbias, 6) << '\n';

			// Return Kalman filter state and covariance.
			else if (c == 'k')
			{
				Serial << _FLOAT(state[0], 6) << ' ' << _FLOAT(state[1], 6) << ' ' << 
					_FLOAT(state[2], 6) << ' ' << _FLOAT(state[3], 6) << ' ' <<
					_FLOAT(state[4], 6) << ' ' << _FLOAT(state[5], 6) << '\n';
			}
		}

		// Kalman filter removes noise from measurements and estimates the new
		// state (linear).
		ktime = kalman(state, covar, ktime);

		// Compute rest of the DOF.
		current[F] = state[0];
		current[H] = state[2];
		current[V] = (analogRead(NPIN) - 230.0)/65.0;
		current[Y] = ahrs_att((enum att_axis) (YAW));
		current[P] = ahrs_att((enum att_axis) (PITCH));
		current[R] = ahrs_att((enum att_axis) (ROLL));
	}
}

void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

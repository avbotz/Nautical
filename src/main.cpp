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

	// Setup motors.
	Motors motors;
	motors.p = 0.0f;

	// Setup kalman filter, including state and covariance.
	Kalman kalman;
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

	// Current state represents location, while desired state holds destination.
	float current[DOF] = { 0.0f };
	float desired[DOF] = { 0.0f };

	// Initial time, helps compute time difference.
	uint32_t ktime = micros();
	uint32_t mtime = micros();

	while (true)
	{
		// Important, otherwise data is sent too fast.
		ahrs_att_update();

		// Check for user input.
		if (Serial.available() > 0)
		{
			char c = Serial.read();

			// Return accelerometer bias. 
			if (c == 'b')
				Serial << _FLOAT(kalman.afbias, 6) << ' ' << _FLOAT(kalman.ahbias, 6) << '\n';

			// Return Kalman filter state and covariance.
			else if (c == 'k')
			{
				Serial << _FLOAT(state[0], 6) << ' ' << _FLOAT(state[1], 6) << ' ' << 
					_FLOAT(state[2], 6) << ' ' << _FLOAT(state[3], 6) << ' ' <<
					_FLOAT(state[4], 6) << ' ' << _FLOAT(state[5], 6) << '\n';
			}

			// Return current state.
			else if (c == 'c')
			{
				Serial << _FLOAT(current[0], 6) << ' ' << _FLOAT(current[1], 6) << ' ' << 
					_FLOAT(current[2], 6) << ' ' << _FLOAT(current[3], 6) << ' ' <<
					_FLOAT(current[4], 6) << ' ' << _FLOAT(current[5], 6) << '\n';
			}

			// Return desired state.
			else if (c == 'd')
			{
				Serial << _FLOAT(desired[0], 6) << ' ' << _FLOAT(desired[1], 6) << ' ' << 
					_FLOAT(desired[2], 6) << ' ' << _FLOAT(desired[3], 6) << ' ' <<
					_FLOAT(desired[4], 6) << ' ' << _FLOAT(desired[5], 6) << '\n';
			}

			// Return motor settings.
			else if (c == 'm')
			{
				Serial << _FLOAT(motors.thrust[0], 6) << ' ' << _FLOAT(motors.thrust[1], 6) << ' ' << 
					_FLOAT(motors.thrust[2], 6) << ' ' << _FLOAT(motors.thrust[3], 6) << ' ' <<
					_FLOAT(motors.thrust[4], 6) << ' ' << _FLOAT(motors.thrust[5], 6) << ' ' << 
					_FLOAT(motors.thrust[6], 6) << ' ' << _FLOAT(motors.thrust[7], 6) << '\n';
			}

			// Return power setting.
			else if (c == 'o')
				Serial << motors.p << '\n';

			// Receive new power setting.
			else if (c == 'p')
				motors.p = Serial.parseFloat();

			// Receive new desired state.
			else if (c == 's')
				for (int i = 0; i < DOF; i++)
					desired[i] = Serial.parseFloat();
		}
		
		// Kalman filter removes noise from measurements and estimates the new
		// state (linear).
		ktime = kalman.compute(state, covar, ktime);
		
		// Compute rest of the DOF.
		current[F] = state[0];
		current[H] = state[2];
		current[V] = (analogRead(NPIN) - 230.0)/65.0;
		current[Y] = ahrs_att((enum att_axis) (YAW));
		current[P] = ahrs_att((enum att_axis) (PITCH));
		current[R] = ahrs_att((enum att_axis) (ROLL));

		// Compute state difference.
		float dstate[DOF] = { 0.0f };
		for (int i = 0; i < BODY_DOF; i++)
			dstate[i] = desired[i] - current[i];

		// Compute PID within motors and set thrust.
		mtime = motors.run(dstate, mtime);
	}
}

void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

#include <Arduino.h>
#include "ahrs/ahrs.h"
#include "dvl/dvl.h"
#include "streaming.h"
#include "config.h"
#include "kalman.hpp"
#include "motor.hpp"
#include "util.hpp"
#include "pid.hpp"
#include "io.hpp"
#include "memory.h"
#include "servo.h"
#include "rotation.h"
#include "voltage.hpp"


void run()
{
	// Start IO between Nautical and hardware.
	if (!SIM) io();

	// Set initial heading to current heading or pool.
	float INITIAL_YAW, INITIAL_PITCH, INITIAL_ROLL; 
	if (!SIM) ahrs_att_update();
	INITIAL_YAW = INITIAL_HEADING ? ahrs_att((enum att_axis) (YAW)) : FAR ? 225.0f : 340.0f; 
	INITIAL_PITCH = ahrs_att((enum att_axis) (PITCH));
	INITIAL_ROLL = ahrs_att((enum att_axis) (ROLL));

	// Store kill state info. 
	bool alive_state = alive();
	bool alive_state_prev = alive_state;
	bool pause = false;
	uint32_t pause_time;

	// Setup motors.
	Motors motors;

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
	float current[DOF] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float desired[DOF] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// Hold difference between the states.
	float dstate[DOF] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// Initial time, helps compute time difference.
	uint32_t ktime = micros();
	uint32_t mtime = micros();

	Serial << "Finished setting up." << endl;

	while (true)
	{
		// Ask AHRS to update. 
		if (!SIM) ahrs_att_update();

		// Ask DVL to update. 
		if (DVL_ON && !SIM) dvl_data_update();

		// Check for user input.
		if (Serial.available() > 0)
		{
			char c = Serial.read();

			// Return alive.
			if (c == 'a')
				Serial << alive() << '\n';

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

			// Return state difference.
			else if (c == 'e')
			{
				Serial << _FLOAT(dstate[0], 6) << ' ' << _FLOAT(dstate[1], 6) << ' ' << 
					_FLOAT(dstate[2], 6) << ' ' << _FLOAT(dstate[3], 6) << ' ' <<
					_FLOAT(dstate[4], 6) << ' ' << _FLOAT(dstate[5], 6) << '\n';
			}

			// Return estimated forces.
			else if (c == 'f')
			{
				Serial << _FLOAT(motors.forces[0], 6) << ' ' << _FLOAT(motors.forces[1], 6) << ' ' << 
					_FLOAT(motors.forces[2], 6) << ' ' << _FLOAT(motors.forces[3], 6) << ' ' <<
					_FLOAT(motors.forces[4], 6) << ' ' << _FLOAT(motors.forces[5], 6) << '\n';
			}

			// Return thrust settings.
			if (c == 't')
			{
				Serial << _FLOAT(motors.thrust[0], 6) << ' ' << _FLOAT(motors.thrust[1], 6) << ' ' << 
					_FLOAT(motors.thrust[2], 6) << ' ' << _FLOAT(motors.thrust[3], 6) << ' ' <<
					_FLOAT(motors.thrust[4], 6) << ' ' << _FLOAT(motors.thrust[5], 6) << ' ' << 
					_FLOAT(motors.thrust[6], 6) << ' ' << _FLOAT(motors.thrust[7], 6) << '\n';
			}

			// Wiimote data or thrust.
			if (c == 'm')
			{
				float thrust[8];
				for (int i = 0; i < 8; i++)
					thrust[i] = Serial.parseFloat();
				m5_power(VERT_FL, thrust[0]);
				m5_power(VERT_FR, thrust[1]);
				m5_power(VERT_BL, thrust[2]);
				m5_power(VERT_BR, thrust[3]);
				m5_power(SURGE_FL, thrust[4]);
				m5_power(SURGE_FR, thrust[5]);
				m5_power(SURGE_BL, thrust[6]);
				m5_power(SURGE_BR, thrust[7]);
				m5_power_offer_resume();
			}

			// Return power setting.
			else if (c == 'o')
				Serial << motors.p << '\n';

			// Receive new power setting.
			else if (c == 'p')
			{
				motors.p = Serial.parseFloat();
				if (motors.p < 0.05f)
				{
					for (int i = 0; i < NUM_MOTORS; i++)
					{
						motors.thrust[i] = 0.0f;
						motors.buttons[i] = 0;
					}
				}
			}
			// Receive new desired state.
			else if (c == 's')
				for (int i = 0; i < DOF; i++)
					desired[i] = Serial.parseFloat();

			// Receive new desired state (relative).
			// Order goes F, H, V then angles.
			else if (c == 'r')
			{
				for (int i = 0; i < DOF; i++)
					desired[i] = current[i];
				float temp1[3];
				float temp2[3] = {current[Y], current[P], current[R]};
				for (int i = 0; i < BODY_DOF; i++)
					temp1[i] = Serial.parseFloat();
				float temp[3];
				body_to_inertial(temp1, temp2, temp);
				for (int i = 0; i < BODY_DOF; i++)
					desired[i] += temp[i];
				for (int i = BODY_DOF; i < GYRO_DOF; i++)
					desired[i] = angle_add(current[i], Serial.parseFloat());
			}

			// Get raw heading.
			else if (c == 'h' && !SIM)
				Serial << ahrs_att((enum att_axis) (YAW)) << '\n';

			// Reset INITIAL_YAW heading, state, and desired state.
			else if (c == 'x' && !SIM)
			{
				state[F] = 0.0;
				state[H] = 0.0;
				desired[F] = 0.0;
				desired[H] = 0.0;
				current[F] = 0.0;
				current[H] = 0.0;
				INITIAL_YAW = ahrs_att((enum att_axis) (YAW));
			}

			// Print the amount of voltage left.
			else if (c == 'v') 
			{
				float voltage = measure_voltage();
				Serial << voltage << '\n';
			}

			// Wiimote buttons.
			else if (c == 'w')
			{
				for (int i = 0; i < NUM_MOTORS; i++)
					motors.buttons[i] = Serial.parseInt();
				if (motors.p > 0.05f) 
				{
					float temp1[3] = {0, 0, 0};
					float temp2[3] = {current[Y], current[P], current[R]};
					if (motors.buttons[0] == 1)
						temp1[0] = 1.25;
					if (motors.buttons[1] == 1)
						temp1[1] = -0.75;
					if (motors.buttons[2] == 1)
						temp1[0] = -1.25;
					if (motors.buttons[3] == 1)
						temp1[1] = 0.75;
					if (motors.buttons[4] == 1)
						desired[Y] = angle_add(desired[Y], -30);
					if (motors.buttons[5] == 1)
						desired[Y] = angle_add(desired[Y], 30);
					float temp[3];
					body_to_inertial(temp1, temp2, temp);
					desired[F] = current[F] + temp[0];
					desired[H] = current[H] + temp[1];
				}
			}
		}

		// Save previous kill state and read new one.
		alive_state_prev = alive_state;
		alive_state = alive();

		// Allow motors to start after pausing.
		if (pause && millis() - pause_time > PAUSE_TIME && !SIM)
			pause = false;

		// Just killed, pause motor communcations.
		if (alive_state_prev && !alive_state && !SIM)
			motors.pause();

		// Just unkilled, allow motors time to restart. 
		if (!alive_state_prev && alive_state &!SIM)
		{ 
			// Set state to 0 when running semis.
			current[F] = 0;
			current[H] = 0;
			state[0] = 0.0;
			state[3] = 0.0;
			pause = true;
			pause_time = millis();
			INITIAL_YAW = ahrs_att((enum att_axis) (YAW)); 
		}

		// Regular running.
		if (SIM || (!pause && alive_state))
		{
			// Kalman filter removes noise from measurements and estimates the new
			// state. Assume angle is correct so no need for EKF.
			float temp1[3] = {current[Y], current[P], current[R]};
			ktime = kalman.compute(state, covar, temp1, ktime);

			// Find sumbarine state.
			current[F] = state[0];
			current[H] = state[3];
			if (!SIM)
			{
				current[V] = (analogRead(NPIN) - 230.0)/65.0;
				current[Y] = ahrs_att((enum att_axis) (YAW)) - INITIAL_YAW;
				current[P] = ahrs_att((enum att_axis) (PITCH)) - INITIAL_PITCH;
				current[R] = ahrs_att((enum att_axis) (ROLL)) - INITIAL_ROLL;
				for (int i = BODY_DOF; i < GYRO_DOF; i++)
					current[i] += (current[i] > 360.0) ? -360.0 : (current[i] < 0.0) ? 360.0 : 0.0;
			}

			// Compute state difference.
			for (int i = 0; i < DOF; i++)
				dstate[i] = 0.0f;

			// Change heading if desired state is far away.
			if (fabs(desired[F]-current[F]) > 3.0 || fabs(desired[H]-current[H] > 3.0))
				desired[Y] = atan2(desired[H]-current[H], desired[F]-current[F]) * R2D;
			desired[Y] += (desired[Y] > 360.0) ? -360.0 : (desired[Y] < 0.0) ? 360.0 : 0.0;

			// Only handle angle if it is bad.
			if (fabs(angle_difference(desired[Y], current[Y])) > 5.0)
			{
				dstate[Y] = angle_difference(desired[Y], current[Y]);
			}
			else 
			{
				dstate[Y] = angle_difference(desired[Y], current[Y]);
				float d0 = desired[F] - current[F];
				float d1 = desired[H] - current[H];
				dstate[F] = d0*cos(D2R*current[Y]) + d1*sin(D2R*current[Y]);
				dstate[H] = d1*cos(D2R*current[Y]) - d0*sin(D2R*current[Y]);
			}
			dstate[V] = desired[V] - current[V];

			// Compute PID within motors and set thrust.
			float temp2[3] = {current[Y], current[P], current[R]};
			mtime = motors.run(dstate, temp2, mtime);
		}
		else 
		{
			ktime = micros();
			mtime = micros();
			if (DVL_ON)
			{
				kalman.m_orig[0] = dvl_get_forward_vel();
				kalman.m_orig[1] = dvl_get_starboard_vel();
			}
		}
	}
}

void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

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
#include "voltage.hpp"


void run()
{
	// Start IO between Nautical and hardware.
    Serial << "Starting Nautical!" << '\n';
    if (!SIM) 
    {
        Serial << "Beginning I/O." << '\n';
        io();
        Serial << "Ending I/O." << '\n';
    }
    else
    {
        Serial << "I/O simulated." << '\n';
    }

	// Set north heading to current heading.
	if (!SIM) 
    {
        Serial << "Beginning AHRS update." << '\n';
        ahrs_att_update();
        Serial << "Ending AHRS update." << '\n';
    }
    else 
    {
        Serial << "AHRS simulated." << '\n';
    }
	float north = FAR ? 225.0f : 340.0f;
    // float north = 235.0f;

	// Store kill state info. 
	bool alive_state = alive();
	bool alive_state_prev = alive_state;
	bool pause = false;
	uint32_t pause_time;
	
    // Setup motors.
	Motors motors;
	motors.p = 0.0f;
    Serial << "Motors setup." << '\n';
    // for (int i = 0; i < NUM_MOTORS; i++)
    //    Serial << motors.thrust[i] << ' '; Serial << '\n';

	// Setup kalman filter, including state and covariance.
    Kalman kalman;
    Serial << "Kalman filter setup." << '\n';
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
    Serial << "States setup." << '\n';

	// Initial time, helps compute time difference.
	uint32_t ktime = micros();
	uint32_t mtime = micros();

    // Serial << "Completed setup." << '\n';
	while (true)
	{
		// Ask AHRS to update. 
        if (!SIM) ahrs_att_update();
            
		// Ask DVL to update. 
        if (DVL_ON && !SIM) 
		    dvl_data_update();
		
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

			// Return thrust settings.
			if (c == 't')
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
			
			// Receive new desired state (relative).
			else if (c == 'r')
			{
				for (int i = 0; i < BODY_DOF; i++)
					desired[i] += Serial.parseFloat();
				for (int i = BODY_DOF; i < GYRO_DOF; i++)
					desired[i] = angle_add(desired[i], Serial.parseFloat());
			}

            // Get raw heading.
            else if (c == 'h' && !SIM)
                Serial << ahrs_att((enum att_axis) (YAW)) << '\n';

			// Reset north heading, state, and desired state.
			else if (c == 'x' && !SIM)
            {
                state[F] = 0.0;
                state[H] = 0.0;
                desired[F] = 0.0;
                desired[H] = 0.0;
                current[F] = 0.0;
                current[H] = 0.0;
				north = ahrs_att((enum att_axis) (YAW));
            }

            // Print the amount of voltage left.
            else if (c == 'v') 
            {
                float voltage = measure_voltage();
                Serial << voltage << '\n';
            }
        }

        // Serial << getFreeMemory() << " " << getMinMemory() << '\n';
    
		// Save previous kill state and read new one.
		alive_state_prev = alive_state;
		alive_state = alive();
            
		// Allow motors to start after pausing.
		if (pause && millis() - pause_time > PAUSE_TIME && !SIM)
        {
            for (int i = 0; i < 8; i++)
            {
                if (isnan(motors.thrust[i]))
                {
                    Serial << "failed motor thrust 1" << '\n';
                }
                break;
            }   

			// north = ahrs_att((enum att_axis) (YAW)); 
			pause = false;
        }

		// Just killed, pause motor communcations.
		if (alive_state_prev && !alive_state && !SIM)
        {
            for (int i = 0; i < 8; i++)
            {
                if (isnan(motors.thrust[i]))
                {
                    Serial << "failed motor thrust 2" << '\n';
                }
                break;
            }   
			motors.pause();
        }

		// Just unkilled, allow motors time to restart. 
		if (!alive_state_prev && alive_state &!SIM)
		{
            for (int i = 0; i < 8; i++)
            {
                if (isnan(motors.thrust[i]))
                {
                    Serial << "failed motor thrust 3" << '\n';
                }
                break;
            }   
            
            // Set state to 0 when running semis.
            current[F] = 0;
            current[H] = 0;
            state[0] = 0.0;
            state[3] = 0.0;
			pause = true;
			pause_time = millis();
			north = ahrs_att((enum att_axis) (YAW)); 
		}

		// Regular running.
		if (SIM || (!pause && alive_state))
		{
			// Kalman filter removes noise from measurements and estimates the new
			// state. Assume angle is correct so no need for EKF.
			ktime = kalman.compute(state, covar, current[Y], ktime);

			// Find sumbarine state.
			current[F] = state[0];
			current[H] = state[3];
            if (!SIM)
            {
			    current[V] = (analogRead(NPIN) - 230.0)/65.0;
                current[Y] = ahrs_att((enum att_axis) (YAW)) - north;
                current[Y] += (current[Y] > 360.0) ? -360.0 : (current[Y] < 0.0) ? 360.0 : 0.0;
                current[P] = ahrs_att((enum att_axis) (PITCH));
                current[R] = ahrs_att((enum att_axis) (ROLL));
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
			mtime = motors.run(dstate, mtime);
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

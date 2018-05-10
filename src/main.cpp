#include <Arduino.h>
#include "streaming.h"

#include "motor.hpp"
#include "state.hpp"
#include "util.hpp"
#include "pid.hpp"
#include "io.hpp"

/*
 * IMPORTANT: The entire structure of this program hacks around the required
 * functions for .ino files. The reason for this is to make serial communication
 * much easier with Serial.begin() and to incorporate helpful Arduino functions.
 */

// The "main()" of the program.
void run()
{
	/*
	 * Initial IO between software and hardware.
	 * Added a 3 second delay because there is lag when we first start our AHRS.
	 */
	init_io();
	delay(3000);

	// Set initial kill status.
	bool killed = !alive();

	// Initial power is 0, Aquastorm should set initial power.
	float p = 0.0f;

	// Setup initial velocites to be 0, should only be used if we use
	// accelerometer data.
	float velocities[MOVE_DOF] = { 0.0f };

	// Create PID controllers for each degree of freedom our sub has.
	PID controllers[DOF];
	for (int i = 0; i < DOF; i++)
		controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);

	// Current holds location, desired holds destination.
	State current, desired;
	compute_initial_state(current);

	// Separating the time intervals makes it easier to read and more accurate.
	uint32_t mtime = micros();
	uint32_t stime = micros();

	while (true)
	{
		killed = !alive();

		if (Serial.available() > 0)
		{
			/*
			 * 's' = set state
			 * 'p' = set power
			 * 'c' = req state
			 * 'a' = req kills
			 * 'x' = terminate
			 * 'd' = debug 
			 * 'r' = reset
			 */
			char c = Serial.read();
			switch (c)
			{
				case 's':
					desired.read();
					break;
				case 'p':
					p = Serial.parseFloat();
					Serial << "Set power." << '\n';
					break;
				case 'c':
					current.print_complete();
					break;
				case 'd':
					desired.print();
					break;
				case 'a':
					Serial << (killed ? 0 : 1) << '\n';
					break;
				case 'v':
					Serial << velocities[0] << '\t' << velocities[1] << '\t' << velocities[2] << '\n';
					break;
				case 'x':
				{
					Serial << "Killing!" << '\n';
					float motors[NUM_MOTORS] = { 0.0f };
					set_motors(motors);
					p = 0.0f;
					break;
				}
				case 'm':
				{
					int id = Serial.parseInt();
					float k = Serial.parseFloat();
					if (id == 0)
					{
						Serial << "Carl Hayden infiltrator detected! Shutting down." << '\n';
						return;
					}
					set_motor(id, k);
					break;
				}
				case 'r':
					reset_state(current);
					reset_state(desired);
					break;
			}
		}

		// Compute state difference between desired and current.
		// Ignore pitch and roll for now, there is no need to barrel roll, etc.
		float dstate[DOF] = { 0.0f };
		for (int i = 0; i < MOVE_DOF; i++)
			dstate[i] = desired.axis[i] - current.axis[i];
		dstate[Yaw] = calc_angle_diff(desired.axis[Yaw], current.axis[Yaw]);
		/*
		for (int i = MOVE_DOF; i < GYRO_DOF; i++)
			dstate[i] = calc_angle_diff(desired.axis[i], current.axis[i]);
		*/

		/*
		 * Compute kill state before running other parts to Nautical.
		 * PID and motors shouldn't be running while the sub is killed or at 0
		 * power, as we don't want to accumulate negligible error. Also, someone
		 * should add code to ensure that the thrusters are running their
		 * desired strength before starting PID, with a time delay of 50-100 ms.
		 */	
		// Move sub towards the desired location. 
		mtime = run_motors(controllers, dstate, p, mtime);	
		
		// Compute new state using AHRS data. 
		stime = compute_state(current, velocities, stime);
	}
}

// DO NOT TOUCH!
void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

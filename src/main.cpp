#include <Arduino.h>
#include "streaming.h"

#include "motor.hpp"
#include "state.hpp"
#include "pid.hpp"
#include "io.hpp"

/*
 * IMPORTANT: The entire structure of this program hacks around the required
 * functions for .ino files. The reason for this is to make serial communication
 * much easier with Serial.begin() and to incorporate helpful Arduino functions.
 */

/*
 * Treat this as main().
 */
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
	float p = 0.f;

	// Create PID controllers for each degree of freedom our sub has.
	PID controllers[DOF];
	for (int i = 0; i < DOF; i++)
		controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);

	// Current holds location, desired holds destination.
	State current, desired;
	compute_initial_state(current);

	while (true)
	{
		unsigned long start = micros();
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
				{
					desired.read();
					break;
				}
				case 'p':
				{
					p = Serial.parseFloat();
					break;
				}
				case 'c':
				{
					current.print_complete();
					break;
				}
				case 'a':
				{
					Serial << (alive() ? 1:0) << '\n';
					break;
				}
				case 'x':
				{
					Serial << "Killing!" << '\n';
					p = 0;
					break;
				}
				case 'd':
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
				{
					reset_state(current);
					reset_state(desired);
					break;
				}
			}
		}

		/*
		 * Compute kill state before running other parts to Nautical.
		 * PID and motors shouldn't be running while the sub is killed or at 0
		 * power, as we don't want to accumulate negligible error. Also, someone
		 * should add code to ensure that the thrusters are running their
		 * desired strength before starting PID, with a time delay of 50-100 ms.
		 */
		// killed = !alive();
			
		// Move sub towards the desired location. 
		run_motors(current, desired, controllers, p, start);	
		
		// Compute new state using AHRS data. 
		compute_state(current, desired, start, p);
	}
}

/*
 * Don't change this!
 */
void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

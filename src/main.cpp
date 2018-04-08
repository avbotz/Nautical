#include <Arduino.h>
#include "streaming.h"

#include "motor.hpp"
#include "state.hpp"
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
	Serial << "Beginning Cuckical!" << '\n';

	// Initialize IO between hardware and software 
	init_io();

	// Initial power is 0
	float p = 0.f;

	// Intialize motors 
	// Motor motors* = init_motors();
	float motors[NUM_THRUSTERS] = { 0.f };

	// Current holds location, desired holds destination
	State current, desired;

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
			 * 'd' =
			 */
			char c = Serial.read();
			switch (c)
			{
				case 's':
				{
					desired.read();
					Serial << desired.x << " " << desired.y << " " << desired.z << '\n';
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
					float powers[NUM_THRUSTERS] = { 0.f };
					set_powers(powers);
					p = 0.f;
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
					Motor m = { (enum thruster) id, k };
					set_motor(m);
					break;
				}
			}
		}

		// Move sub towards the desired location 
		run_motors(current, desired, motors, p);	

		// Compute new state using AHRS data 
		compute_state(current, start);
		// delay(500);
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

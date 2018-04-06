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
	Serial << "Beginning Nautical!" << '\n';

	// Initialize IO between hardware and software 
	init_io();

	// Initial power is 0
	float p = 0.f;

	// Intialize motors 
	Motor *motors = init_motors();

	// Current holds location, desired holds destination
	State current, desired;

	while (true)
	{
		if (Serial.available() > 0)
		{
			/*
			 * 's' = set state 
			 * 'p' = set power
			 * 'c' = req state 
			 * 'a' = req kills
			 * 'x' = terminate
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
					current.print();
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
					break;
				}
				case 't':
				{
					int x = Serial.parseInt();
					float k = Serial.parseFloat();
					Motor blah = { (enum thruster) x, k };
					set_motor(blah);
					break;
				}
			}
		}

		// Move sub towards the desired location 
		run_motors(current, desired, motors);	

		// Compute new state using AHRS data 
		// TODO	
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

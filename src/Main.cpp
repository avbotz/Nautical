#include <Arduino.h>

#include "Streaming.h"
#include "State.hpp"

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
	
	/*
	 * Current state holds where the sub thinks it is at.
	 * Desired state holds where the sub wants to be.
	 */
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
			 */
			char c = Serial.read();

			switch (c)
			{
				case 's':
				{
					desired.read();
					desired.print();
					break;
				}
				case 'p':
				{
					char k = Serial.read();
					break;
				}
				case 'c': 
				{	
					break; 
				}
				case 'a':
				{
					break;
				}
			}
		}
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

#include <Arduino.h>
#include "Streaming.h"

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


	while (true)
	{
		if (Serial.available() > 0)
		{
			/*
			 * 's' = setting state 
			 * 'p' = setting power
			 * 'c' = request state 
			 * 'a' = request kill 
			 */
			char c = Serial.read();

			switch (c)
			{
				case 's':
				{
					String desiredState = Serial.readStringUntil('\n');
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

#include <Arduino.h>
#include "Streaming.h"

/*
 * IMPORTANT: The entire structure of this program hacks around the required
 * functions for .ino files. The reason for this is to make serial communication
 * much easier with Serial.begin() and to incorporate helpful Arduino functions.
 */

/*
 * Treat run() as main().
 */
void run()
{
	Serial << "Beginning Nautical!" << '\n';

	while (true)
	{
		// Receive input through /dev/tty* port 
		if (Serial.available() > 0)
		{
			String input = Serial.readStringUntil('\n');
			Serial << "Received input! " << input << '\n';
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

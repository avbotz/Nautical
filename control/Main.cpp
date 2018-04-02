#include <Arduino.h> 
#include <Wire.h>
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
	while (true)
	{
		Serial << "Cuck" << '\n';
		delay(1000);
	}
}

/*
 * Don't change this!
 */ 
void setup() 
{ 
	Serial.begin(115200);
	run();
}
void loop() {}

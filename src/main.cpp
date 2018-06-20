#include <Arduino.h>
#include "streaming.h"

#include "motor.hpp"
#include "util.hpp"
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
	// Serial << "Beginning Cuckical!" << '\n';
	init_io();

	// Set initial kill status.
	bool killed = !alive();

	// Initial power is 0, Aquastorm should set initial power.
	float p = 0.0f;

	// Create PID controllers for each degree of freedom our sub has.
	PID controllers[DOF];
	for (int i = 0; i < DOF; i++)
		controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);

	// Current holds location and desired holds destination.
	State current, desired;
	compute_initial_state(current);

	// Setup calculations for time difference. 
	uint32_t start = micros();
	uint32_t ptime = start;
	uint32_t stime = start;

	while (true)
	{
		delay(10);
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
						current.print();
						break;
					}
				case 'd':
					{
						desired.print();
						break;
					}
				case 'a':
					{
						Serial << (alive() ? 1:0) << '\n';
						break;
					}
				case 'x':
					{
						p = 0;
						float temp[NUM_MOTORS] = { 0.0f };
						set_motors(temp);
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
					{
						reset_state(current);
						reset_state(desired);
						break;
					}
			}
		}


		// Compute state difference.
		// Managing multiple axes at one time increases drift in the sub, so it
		// will be avoided until the DVL is integrated.
		float dstate[DOF] = { 0.0f };
		if (fabs(desired.z - current.z) > 0.05)
			dstate[2] = desired.z - current.z;	
		else
		{
			dstate[0] = desired.x - current.x;
			dstate[1] = desired.y - current.y;
			dstate[3] = calc_angle_diff(desired.yaw, current.yaw);
		}

		// Compute PID using state difference.
		float dt = (float)(micros() - ptime)/(float)(1000000);	
		float pid[DOF] = { 0.0f };
		for (int i = 0; i < MOVE_DOF; i++)
			pid[i] = controllers[i].calculate(dstate[i], dt, 0.25);		
		pid[3] = controllers[3].calculate(dstate[3], dt, 0.025);
		ptime = micros();

		// Compute new state using AHRS data. 
		stime = compute_state(current, pid, p, stime);

		// Move sub towards the desired location. 
		compute_motors(dstate, pid, p);	
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

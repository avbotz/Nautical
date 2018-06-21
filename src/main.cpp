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
	init_io();

	float p = 0.0f;
	PID controllers[DOF];
	for (int i = 0; i < DOF; i++)
		controllers[i].init(GAINS[i][0], GAINS[i][1], GAINS[i][2]);

	uint32_t start = micros();
	uint32_t ptime = start;
	uint32_t stime = start;

	float *current = new float[DOF];
	float *desired = new float[DOF];

	while (true)
	{
		delay(10);
		if (Serial.available() > 0)
		{
			char c = Serial.read();
			if ()
			}
		}

		float dstate[DOF] = { 0.0f };
		if (fabs(desired.z - current.z) > 0.05)
			dstate[2] = desired.z - current.z;	
		else
		{
			dstate[0] = desired.x - current.x;
			dstate[1] = desired.y - current.y;
			dstate[3] = calc_angle_diff(desired.yaw, current.yaw);
		}

		float dt = (float)(micros() - ptime)/(float)(1000000);	
		float pid[DOF] = { 0.0f };
		for (int i = 0; i < MOVE_DOF; i++)
			pid[i] = controllers[i].calculate(dstate[i], dt, 0.25);		
		pid[3] = controllers[3].calculate(dstate[3], dt, 0.025);
		ptime = micros();

		stime = compute_state(current, pid, p, stime);

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

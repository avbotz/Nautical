#include <Arduino.h>
#include "ahrs/ahrs.h"
#include "streaming.h"
#include "config.h"

#include "kalman.hpp"
#include "motor.hpp"
#include "util.hpp"
#include "pid.hpp"
#include "io.hpp"


void run()
{
	io();

	float p = 0.0f;

	float *current = new float[DOF];
	float *desired = new float[DOF];

	float *state = new float[N]; 
	float *covar = new float[N*N];
	
	afbias = ahrs_accel((enum accel_axis)(SURGE));
	ahbias = ahrs_accel((enum accel_axis)(SWAY));

	uint32_t ktime = micros();

	while (true)
	{
		delay(10);

		ktime = kalman(state, covar, ktime);
	}
}

void setup()
{
	Serial.begin(9600);
	run();
}
void loop() {}

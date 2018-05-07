#include "pid.hpp"
#include "util.hpp"


float PID::init(float a, float b, float c)
{
	this->kp = a;
	this->ki = b;
	this->kd = c;
}

float PID::calculate(float error, float dt)
{
	// P, I, and D calculations using time difference dt.
	float pout = this->kp * error;
	this->sum += error * dt;
	float iout = this->ki * this->sum;
	float dout = this->kd * (error - this->prev) / dt;
	this->prev = error;

	// Limit out to be between 0.25 and 2.
	// Sometimes, the sub will slow down too quickly once it reaches within 5
	// degrees of it's gyro target, so this should ensure that it can still move
	// at a reasonable pace. The upper bound is to prevent freak accidents.
	float out = pout + iout + dout;
	return limit(out, 0.25, 2);
}

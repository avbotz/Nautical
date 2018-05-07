#include "pid.hpp"


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

	float out = pout + iout + dout;
	return out;
}

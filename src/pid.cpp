#include "pid.hpp"

float PID::init(float a, float b, float c)
{
	this->kp = a;
	this->ki = b;
	this->kd = c;
}

float PID::calculate(float error, float dt)
{
	// Proportional
	float pout = this->kp * error;

	// Integral
	this->sum += error * dt;
	float iout = this->ki * this->sum;

	// Derivative
	float dout = this->kd * (error - this->prev) / dt;
	this->prev = error;

	return pout + iout + dout;
}

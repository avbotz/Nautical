#include "pid.hpp"
#include "util.hpp"


float PID::init(float a, float b, float c)
{
	this->kp = a;
	this->ki = b;
	this->kd = c;
}

float PID::calculate(float error, float dt, float min)
{
	float pout = this->kp * error;
	this->sum += error * dt;
	float iout = this->ki * this->sum;
	float dout = this->kd * (error - this->prev) / dt;
	this->prev = error;
	float output = pout + iout + dout;
	float dir = 1.0;
	if (output < 0) dir = -1.0;

	float out = limit(limit(output, -1.5f, 1.5f), min);
	return out;
}

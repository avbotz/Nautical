#include "pid.hpp"
#include "util.hpp"


float PID::init(float a, float b, float c)
{
	this->kp = a;
	this->ki = b;
	this->kd = c;
	this->prev = 0.0;
	this->sum = 0.0;
}

/*
 * IMPORTANT: Make sure DT isn't 0, will cause motors to send NAN.
 */
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

	float out = limit(limit(output, -2.0f, 2.0f), min);
	return out;
}

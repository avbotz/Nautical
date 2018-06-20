#ifndef PID_HPP
#define PID_HPP 

struct PID 
{
	float kp, ki, kd;
	float prev, sum;
	PID() {}
	PID(float a, float b, float c) : kp(a), ki(b), kd(c), prev(0.f), sum(0.f) {}

	float init(float a, float b, float c);
	float calculate(float error, float dt, float lowest);
};

#endif 

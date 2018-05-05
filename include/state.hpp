#ifndef STATE_HPP
#define STATE_HPP 

struct State 
{
	/*
	 * D | +/- 	
	 * X = forward/backward
	 * Y = right/left 
	 * Z = down/up
	 * Yaw, Pitch, and Roll are obvious.
	 * Surge 	= forward/backward
	 * Sway 	= right/left
	 * Heave 	= down/up
	 */
	float x, y, z;
	float yaw, pitch, roll;
	float ax, ay, az;
	State() : x(0), y(0), z(0), yaw(0), pitch(0), roll(0), ax(0), ay(0), az(0) {}

	void read();
	void print();
	void print_complete();
};

void compute_state(struct State &state, struct State &desired, unsigned long start, float p);

#endif 

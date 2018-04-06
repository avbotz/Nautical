#ifndef IO_HPP
#define IO_HPP

#include "state.hpp"
#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

void init_io();
void set_powers(float vals[NUM_THRUSTERS]);
void set_motor(Motor s);
bool alive();
State get_state();

#endif

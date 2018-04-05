#ifndef IO_HPP
#define IO_HPP

#include "state.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

void init_io();
State getState();
bool alive();
void setpowers(float vals[NUM_THRUSTERS]);

#endif

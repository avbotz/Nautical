#ifndef IO_HPP
#define IO_HPP

#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

#define NPIN A0
#define KILL_PIN 30

void io();
bool alive();

#endif

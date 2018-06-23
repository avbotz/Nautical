#ifndef IO_HPP
#define IO_HPP

#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

// depth pin
#define NPIN A0

// kill pin
#define KILL_PIN 30

// void toggleRelay();
void io();
bool alive();

#endif

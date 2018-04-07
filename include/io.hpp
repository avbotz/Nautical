#ifndef IO_HPP
#define IO_HPP

#include "state.hpp"
#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

// depth pin
#define NPIN A0
// relay pins
#define DROPPER_PIN 53
#define GRABBER_L_PIN 51
#define GRABBER_R_PIN 49
#define TORPEDO_L_PIN 47
#define TORPEDO_R_PIN 45

void init_io();
bool alive();

#endif

#include <Arduino.h>
#include "streaming.h"
#include "state.hpp"

void State::read()
{
	this->x = Serial.parseFloat();
	this->y = Serial.parseFloat();
	this->z = Serial.parseFloat();
	this->yaw 	= Serial.parseFloat();
	this->pitch = Serial.parseFloat();
	this->roll 	= Serial.parseFloat();
}

void State::print()
{
	Serial << this->x << " " << this->y << " " << this->z << " " << 
		this->yaw << " " << this->pitch << " " << this->roll << '\n';
}

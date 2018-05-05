#include <Arduino.h>
#include "streaming.h"

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "m5/io_m5.h"
#include "m5/m5.h"
#include "macrodef.h"
#include "io.hpp"
#include "motor.hpp"
#include "state.hpp"


void init_io()
{
	// Init communication with the AHRS
	io_ahrs_init("");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	// Init communication with the kill switch
	pinMode(KILL_PIN, INPUT);
	
	// Init communication with depth
	pinMode(NPIN, INPUT);

	// Init communication with the M5
	io_m5_init("");
	io_m5_trans_set(m5_power_trans);	
}

bool alive()
{
	return digitalRead(KILL_PIN) ? false : true;
}

/*
int getRelayState(int pin)
{
	for (int i = 0; i < 5; i++)
		if (relayStates[i][0] == pin)
			return relayStates[i][1];
	
	return -1;
}

void setRelayState(int pin, int state)
{
	for (int i = 0; i < 5; i++)
		if (relayStates[i][0] == pin)
			relayStates[i][1] == state;
}

void toggleRelay(int pin)
{
	int state = getRelayState(pin);
	if (state == 1)
	{
		digitalWrite(pin, LOW);
		setRelayState(pin, 0);
	}
	else if (state == 0)
	{
		digitalWrite(pin, HIGH);
		setRelayState(pin, 1);
	}
	else if (state == -1)
	{
		Serial << "error: invalid pin" << '\n';
		return;
	}
}
*/

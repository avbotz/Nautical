#include <Arduino.h>
#include <math.h>
#include <stdio.h>

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "m5/io_m5.h"
#include "m5/m5.h"
#include "macrodef.h"
#include "io.hpp"
#include "motor.hpp"
#include "state.hpp"

#define KILL_PIN 30

int relayStates[5][2];

void init_io()
{
	// Init communication with the AHRS
	io_ahrs_init("");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	// Init communication with the kill switch
	pinMode(KILL_PIN, INPUT);
	// Init communicatino with depth
	pinMode(NPIN, INPUT);

	// Init communicatin with relays
	pinMode(DROPPER_PIN, OUTPUT);
	pinMode(GRABBER_L_PIN, OUTPUT);
	pinMode(GRABBER_R_PIN, OUTPUT);
	pinMode(TORPEDO_L_PIN, OUTPUT);
	pinMode(TORPEDO_R_PIN, OUTPUT);

	// Init relay state map
	relayStates[0][0] = DROPPER_PIN;
	relayStates[1][0] = GRABBER_L_PIN;
	relayStates[2][0] = GRABBER_R_PIN;
	relayStates[3][0] = TORPEDO_L_PIN;
	relayStates[4][0] = TORPEDO_R_PIN;

	// Init communication with the M5
	float powers[NUM_THRUSTERS] = { 0.f };
	io_m5_init("");
	set_powers(powers);
	io_m5_trans_set(m5_power_trans);
}

bool alive()
{
	return digitalRead(KILL_PIN) ? false : true;
}

int getRelayState(int pin)
{
	for (int i = 0; i < 5; i++)
	{
		if (relayStates[i][0] == pin)
		{
			return relayStates[i][1];
		}
	}
	return -1;
}

void setRelayState(int pin, int state)
{
	for (int i = 0; i < 5; i++)
	{
		if (relayStates[i][0] == pin)
		{
			relayStates[i][1] == state;
		}
	}
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
		std::cout << "error: invalid pin" << std::endl;
		return;
	}
}

#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include <map>

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "m5/io_m5.h"
#include "m5/m5.h"
#include "macrodef.h"
#include "io.hpp"
#include "motor.hpp"
#include "state.hpp"

#define KILL_PIN 30

std::map<int, bool> relayStates;

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
	relayStates.insert(pair<int,int>(DROPPER_PIN, false));
	relayStates.insert(pair<int,int>(GRABBER_L_PIN, false));
	relayStates.insert(pair<int,int>(GRABBER_R_PIN, false));
	relayStates.insert(pair<int,int>(TORPEDO_L_PIN, false));
	relayStates.insert(pair<int,int>(TORPEDO_R_PIN, false));

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

void toggleRelay(int pin) {
	auto state = relayStates.find(pin);
	if (state != relayStates.end()) {
		if (state->second == true) {
			digitalWrite(pin, LOW);
			relayStates.insert(pair<int,int>(pin, false));
		} else {
			digitalWrite(pin, HIGH);
			relayStates.insert(pair<int,int>(pin, true));
		}
	} else {
		std::cout << "error: invalid pin" << std::endl;
		return;
	}
}

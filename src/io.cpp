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

void init_io()
{
	// Init communication with the AHRS
	io_ahrs_init("");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	// Init communication with the kill switch 
	pinMode(KILL_PIN, INPUT);

	// Init communication with the M5
	io_m5_init("");
	io_m5_trans_set(m5_power_trans);	
}

bool alive()
{
	return digitalRead(KILL_PIN) ? false : true;
}

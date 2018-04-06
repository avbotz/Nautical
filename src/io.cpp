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
	// Initialize communication with the AHRS
	io_ahrs_init("");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	// Initialize communication with the kill switch 
	pinMode(KILL_PIN, INPUT);

	// Initialize communication with the M5
	float powers[NUM_THRUSTERS] = { 0.f };
	io_m5_init("");
	set_powers(powers); 
	io_m5_trans_set(m5_power_trans);	
}

bool alive()
{
	return digitalRead(KILL_PIN) ? false : true;
}

State get_state()
{
	ahrs_att_update();
	State out;
	out.yaw = ahrs_att((enum att_axis)(YAW));
	out.pitch = ahrs_att((enum att_axis)(PITCH));
	out.roll = ahrs_att((enum att_axis)(ROLL));

	out.sway = ahrs_accel((enum accel_axis)(SWAY));
	out.heave = ahrs_accel((enum accel_axis)(HEAVE));
	out.surge = ahrs_accel((enum accel_axis)(SURGE));

	return out;
}

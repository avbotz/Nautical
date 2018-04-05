#include <math.h>
#include <stdio.h>

#include "io.hpp"

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"

#include "m5/io_m5.h"
#include "m5/m5.h"

#include "state.hpp"
#include "macrodef.h"

#define KILL_PIN 30

void init_io(){
	//AHRS
	io_ahrs_init("/dev/ttyUSB0");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv); // Start receiving data asynchronously

	//KILL SWITCH
	pinMode(KILL_PIN, INPUT);

	Serial << "d";

	//M5
	io_m5_init("/dev/ttyUSB1");
	float powers[NUM_THRUSTERS] = {0.f};
	setpowers(powers); // zero powers
	io_m5_trans_set(m5_power_trans);	

	Serial << "k";
}

bool alive(){
	return digitalRead(KILL_PIN) ? false : true;
}

void setpowers(float vals[NUM_THRUSTERS])
{
	for (uint_fast8_t t = NUM_THRUSTERS; t--;)
	{
		// zero powers
		m5_power((enum thruster)t, vals[t]);
	}
}

State getState(){
	ahrs_att_update();
	State out;
	out.yaw = ahrs_att((enum att_axis)(YAW));
	out.pitch = ahrs_att((enum att_axis)(PITCH));
	out.roll = ahrs_att((enum att_axis)(ROLL));

	out.sway = ahrs_accel((enum accel_axis)(SWAY));
	out.heave = ahrs_accel((enum accel_axis)(HEAVE));
	out.surge = ahrs_accel((enum accel_axis)(SURGE));

	out.hstatus = ahrs_headingstatus();

	return out;
}

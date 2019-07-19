#include <Arduino.h>
#include "config.h"
#include "streaming.h"
#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "m5/io_m5.h"
#include "m5/m5.h"
#include "dvl/io_dvl.h"
#include "dvl/dvl.h"
#include "macrodef.h"
#include "io.hpp"
#include "motor.hpp"


void io()
{
	io_ahrs_init("/dev/ttyUSB0");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	pinMode(KILL_PIN, INPUT);

	pinMode(DEPTH_PIN, INPUT);
	pinMode(49, OUTPUT);
	digitalWrite(49, HIGH); 

	if (DVL_ON)
	{
		pinMode(19, INPUT);  
		pinMode(A5, INPUT);
		digitalWrite(19, HIGH);
		io_dvl_init(dvl_receive_handler);
		io_dvl_recv_begin();
		dvl_set_data_format();
		dvl_begin_pinging();
	}

	io_m5_init("");
	io_m5_trans_set(m5_power_trans);	
}

bool alive()
{
	if (SIM) return true;
	return digitalRead(KILL_PIN) ? false : true;
}


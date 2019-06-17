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

ServoTimer2 dropper1, dropper2;


void io()
{
	// Start communication with the AHRS.
	io_ahrs_init("/dev/ttyUSB0");
	ahrs_set_datacomp();
	ahrs_cont_start();
	io_ahrs_recv_start(ahrs_att_recv);

	// Start communication with the kill switch.
	pinMode(KILL_PIN, INPUT);

	// Start communication with depth.
	pinMode(NPIN, INPUT);

	pinMode(49, OUTPUT);
	digitalWrite(49, HIGH); 

	// Setup droppers. 
	dropper1.attach(8);
	dropper2.attach(9);
	dropper1.write(2400);
	dropper2.write(2400);

	// Start communication with DVL.
	if (DVL_ON)
	{
		pinMode(19, INPUT);  
		pinMode(A5, INPUT);
		digitalWrite(19, HIGH);
		io_dvl_init(dvl_receive_handler);
		io_dvl_recv_begin();
		// delay(5000);
		dvl_set_data_format();
		dvl_begin_pinging();
	}

	// Start communication with the M5. 
	io_m5_init("");
	io_m5_trans_set(m5_power_trans);	
}

void drop(int idx)
{
	if (idx == 0)
	{
		int angle = dropper1.read();
		if (angle > 1700)
			dropper1.write(600);
		if (angle <= 1700)
			dropper1.write(2700);
		// Serial.println(dropper1.read());
	}
	else
	{
		int angle = dropper2.read();
		if (angle > 1700)
			dropper2.write(600);
		if (angle <= 1700)
			dropper2.write(2400);
		// Serial.println(dropper2.read());
	}
}

bool alive()
{
	if (SIM) return true;
	return digitalRead(KILL_PIN) ? false : true;
}


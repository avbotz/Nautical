/*
 * MIT License
 * 
 * Copyright (c) 2019 AVBotz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ========================================================================== */

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

ServoTimer2 dropper1;
ServoTimer2 dropper2; 


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

	dropper1.attach(6);
	dropper2.attach(7);
	dropper1.write(2700);
	dropper2.write(2700);
	
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

void drop(int idx, int val)
{
	if (idx == 0)
	{
		int angle = dropper1.read();
		if (val == 0) dropper1.write(2400);
		if (val == 1) dropper1.write(700);
	}
	else if (idx == 1)
	{
		int angle = dropper2.read();
		if (val == 0) dropper2.write(2400);
		if (val == 1) dropper2.write(700);
	}
}

bool alive()
{
	if (SIM) return true;
	return digitalRead(KILL_PIN) ? false : true;
}


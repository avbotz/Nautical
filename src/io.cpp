#include <Arduino.h>
#include "streaming.h"

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "m5/io_m5.h"
#include "m5/m5.h"
#include "macrodef.h"
#include "io.hpp"
#include "motor.hpp"

ServoTimer2 dropper1, dropper2;

void io()
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

	pinMode(49, OUTPUT);
	digitalWrite(49, HIGH); 

    dropper1.attach(8);
    dropper2.attach(9);

    dropper1.write(2400);
    dropper2.write(2400);

	// Init communication with the M5
	io_m5_init("");
	io_m5_trans_set(m5_power_trans);	
}

void drop(int idx){
    if(idx == 0){
        int angle = dropper1.read();
        if(angle > 1700)
            dropper1.write(544);
        if(angle <= 1700)
            dropper1.write(2700);
        Serial.println(dropper1.read());
    }
    else{
        int angle = dropper2.read();
        if(angle > 1700)
            dropper2.write(544);
        if(angle <= 1700)
            dropper2.write(2400);
        Serial.println(dropper2.read());
    }
}

bool alive()
{
	return digitalRead(KILL_PIN) ? false : true;
}


#include <Arduino.h>
/*
 * Battery Voltage Checking code as writtin by Maxim Vovenko from electrical
 * Honestly I used some of my old code that did work but altered it so that it should be able to read multiple voltages
 * Havent checked to see if it works but it dose compile so...
 * 
 * Make sure to set calibration values before uploding else our battery will run at 5 volts?
 * if the voltage devider is 1 to 10 ratio then value of 11 should be a good start untill we come up with better values (that will be fun for electrical)
 */

void measure_voltage(){
  #define NUM_SAMPLES 10 //Number of samples per measurment (higher = bettery accuracy but longer time)
  int sumB1 = 0; //used for algebra
  int purgeAnalog = 0;
  float calibrateB1 = 23.88349514563107; //used to calibrate circuit set for a 10 1 voltage devider assuuming 5v reference and other stuff
  float voltageB1=0.0; //used as final volttage output
  unsigned char sample_count = 0;
  //Code to measure voltage of each battery seperatly
  purgeAnalog = analogRead(A7); //preping ardino to read pin A1
  delay(10);
  while (sample_count < NUM_SAMPLES){ //collecting data A1
    sumB1 += analogRead(A7);
    delay(10);
    sample_count++;
    }
  sample_count=0;
  voltageB1 = (((float)sumB1 / (float)NUM_SAMPLES * 5.0) / 1024.0)*(float)calibrateB1; //calculating voltage WORKS!!!
  Serial.print("Battery 1 = "); //returning voltages
  Serial.println(voltageB1);
//  Serial.println(((float)sumB1/(float)NUM_SAMPLES)/1024*5);//CALLIRATION / DEBUG
  
}


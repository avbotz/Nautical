#include <Arduino.h>

void print_voltage(){
  #define NUM_SAMPLES 10 //Number of samples per measurment (higher = bettery accuracy but longer time)
  int sumB1 = 0; //used for algebra
  int purgeAnalog = 0;
  float calibrateB1 = 23.88349514563107; //used to calibrate circuit set for a 10 1 voltage devider assuuming 5v reference and other stuff
  float voltageB1=0.0; //used as final volttage output
  unsigned char sample_count = 0;
  //Code to measure voltage of each battery seperatly
  purgeAnalog = analogRead(A1); //preping ardino to read pin A1
  delay(10);
  while (sample_count < NUM_SAMPLES){ //collecting data A1
    sumB1 += analogRead(A1);
    delay(10);
    sample_count++;
    }
  sample_count=0;

  voltageB1 = (((float)sumB1 / (float)NUM_SAMPLES * 5.0) / 1024.0)*(float)calibrateB1; //calculating voltage WORKS!!!
  Serial.print("Battery Both = "); //returning voltages
  Serial.println(voltageB1);
//  Serial.println(((float)sumB1/(float)NUM_SAMPLES)/1024*5);//CALLIRATION / DEBUG
}

float measure_voltage() {
    uint8_t num_samples = 10;
    float cal_constant = 23.88349514563107;
    float voltage = 0.0f;
    
    analogRead(A1);
    
    for (size_t i = 0; i < num_samples; i++) {
        voltage += analogRead(A1);
        delay(1);
    }

    voltage = ((voltage / num_samples*5.0) / 1024.0) * cal_constant;
    return voltage;
}



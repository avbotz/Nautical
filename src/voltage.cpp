#include <Arduino.h>


float measure_voltage() 
{
	uint8_t num_samples = 10;
	float cal_constant = 23.88349514563107;
	float voltage = 0.;

	analogRead(A1);

	for (size_t i = 0; i < num_samples; i++)
	{
		voltage += analogRead(A1);
		delay(1);
	}

	voltage = voltage/num_samples*5./1024.*cal_constant;
	// voltage = ((voltage / num_samples*5.0) / 1024.0) 
	return voltage;
}



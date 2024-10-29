#include "types.h"

/*
Equations for calculating range(in cm) from voltage:
Volts(Digital Output) = (Analog Reading * (1024/5))
distance in centimeters = (6786/(Volts-3))-4;
*/

// Function to convert ADC value to distance
u8 ADCValueToDiatance(f32 val)
{
	u32 intSensorVal;
	u8 distanceCM;
	
	intSensorVal = (1024/5) * val;
	distanceCM = ((6787/(intSensorVal-3))-4);
	
	if((distanceCM>=15)&&(distanceCM<=32))
		--distanceCM;
	else if((distanceCM>=33)&&(distanceCM<=55))
		distanceCM-=2;
	else if((distanceCM>=56)&&(distanceCM<=79))
		--distanceCM;
	
	return distanceCM;
}

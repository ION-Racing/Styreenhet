#include "SPI.h"
#include "systick.h"

void InitGyro(void){
	
	/* Start communication with AKS/GYRO.
	Must happen after approximately 800ms
	after startup*/
	SPIstartCommunication();	
}



uint16_t gyrodata;

void ReadGyro(void)
{
	if(clk100msSPI == COMPLETE) 
	{
		gyrodata = SPI_ReadData(0x00);
		clk100msSPI = RESTART;
	}
}

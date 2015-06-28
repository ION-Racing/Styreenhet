#include "stm32f4xx.h"
#include "CAN.h"
#include "ION_CAN.h"
#include "Global.h"

uint32_t calculateRpm(uint32_t);
uint32_t frequency;

/*
@Param NONE. Uses global struct wheel. 
wheel values gets set in EXTI.c GPIO interrupt.
------------------------------------------------
Data field 1 & 2 contains wheel sensor 1 data.
Data field 3 & 4 contains wheel sensor 2 data.
*/


void TxWheelrpm(CanTxMsg msg)
{
	__disable_irq();

	uint8_t data[4];

	frequency = calculateRpm(wheel.period1);
	data[0] = frequency;
	data[1] = frequency >> 8;

	frequency = calculateRpm(wheel.period2);
	data[2] = frequency;
	data[3] = frequency >> 8;

	CANTx(CAN_MSG_WHEEL_RPM, 4, data);
	
	wheel.period1 = 0;
	wheel.period2 = 0;

	__enable_irq();
		
}

/* 
@retval 0 if period is 0 otherwise the calculated frequency
*/
uint32_t calculateRpm(uint32_t period)
{
	if(period == 0){
		return 0;
	}
	
	return 1000000 / period; // f*1000 = (1/t[us])*1000
}

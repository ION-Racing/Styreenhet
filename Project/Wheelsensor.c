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

static const uint8_t pulsesPerRevolution = 30;
static const uint16_t wheelCircumference = 100; // Wheel circumference in cm

void TxWheelrpm(void)
{
	__disable_irq();

	uint32_t fLeft = calculateRpm(wheel.period1);
	uint32_t fRight = calculateRpm(wheel.period2);

	// Calculate speed
	uint32_t rpm = fLeft / pulsesPerRevolution * 60;
	uint16_t speed = (rpm * wheelCircumference * 3600) / 100000; // Speed in km/h
	
	uint8_t data[8] = {speed >> 8, speed & 0xFF};
	
	CANTx(CAN_MSG_SPEED, 2, data);
	
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

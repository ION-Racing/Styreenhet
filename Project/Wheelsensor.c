#include "stm32f4xx.h"
#include "Global_variables.h"

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

				//Declare common variables for CAN message.
				msg.StdId = 0x1;
				msg.RTR = CAN_RTR_DATA;
				msg.IDE = CAN_ID_STD;
				msg.DLC = 4;
				
				frequency = calculateRpm(wheel.period1);
				msg.Data[0] = frequency;
				msg.Data[1] = frequency>>8;
				
				frequency = calculateRpm(wheel.period2);
				msg.Data[2] = frequency;
				msg.Data[3] = frequency>>8;
				
				CAN_Transmit(CAN1, &msg);
				wheel.period1 = 0;
				wheel.period2 = 0;
				
				__enable_irq();
			
	}
	/* 
	@retval 0 if period is 0 otherwise the calculated frequency
	*/
	
	uint32_t calculateRpm(uint32_t period){
		if(period == 0)
		{
			return 0;
		}else return 1000000/period; // f*1000 = (1/t[us])*1000
	}
	
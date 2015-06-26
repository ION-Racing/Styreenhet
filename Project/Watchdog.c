#include <stm32f4xx.h>
#include "watchdog.h"

/*
Configuration of the independent watchdog.
Counts down with the prescaled LSI clock from
setReload value. If it counts down to 0x000 
the microcontroller resets itself.
use IWDG_ReloadCounter() to reset the counter.

LSI = 32 Khz.
----------------------------------------
prescaler = 4, setReload=0xFFF -> 512ms
Prescaler = 8, setReload=0xFFF -> 1024ms
*/

void InitWatchdog(void){
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_4); 			
	IWDG_SetReload(0xFFF); 
	IWDG_Enable();
}

void initWatchdogCAN(){
	//init the counter
}

void kickTheCAN()
	{
		//Reset the counter 
}
	
//watchdog interrupt handler for CAN

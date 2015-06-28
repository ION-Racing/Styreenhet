#include <stm32f4xx.h>
#include <stdbool.h>
#include "startup.h"
#include "GPIO.h"
#include "Global.h"


uint16_t RTDS_Time = 0;

uint8_t START_Pushed = 0;

/*uint8_t RTDS(void){
	if( calibrateSensors()){
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		return 1;
	}
	else return 0;

}

uint8_t mcRun(void){
	
	if(RTDS() && pedalSensors[1] > 0xFFF/20){ // If RTDS has been sounded and brake pedal is engaged(>5%).
	
		GPIO_SetBits(GPIOB, GPIO_Pin_12); // Set RUN signal high.

		return 1;
	}
	
	else return 0;
	
}*/

extern CarState carState;
extern uint16_t pedalValues[2];

void checkStartup(void){
	
	//calibrateSensors();
	
	bool brake = pedalValues[PEDAL_BRAKE] > 0xFFF / 2;
	bool start = START_Pushed == 1;
	
	if(carState == DISARMED)
	{
		if(brake && start){
	
			// Start RTDS
			SetRTDS(ENABLE);
			RTDS_Time = 1000; // 1 sec
			
			// Switch state
			carState = ARMING;
		}
	}
	else if(carState == ARMING){
		if(RTDS_Time == 0)
		{
			// Stop RTDS
			SetRTDS(DISABLE);
			
			// Arm car
			carState = ARMED;
		}
	}
	else if(carState == ARMED){
		carState = DISARMED;
	}
	
	START_Pushed = 0; // TODO: Fix this (interrupts)
	
	/*if(brems && !stop){
		
		if(start){
		
			while(precharge == 0){
			
				CAN_TR(start);
				return 1;
					
			}
		}
	}	*/
}

void Startup_START_Pushed(void){
	START_Pushed = 1;
}

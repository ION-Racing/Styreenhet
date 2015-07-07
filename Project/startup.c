#include <stm32f4xx.h>
#include <stdbool.h>
#include "startup.h"
#include "GPIO.h"
#include "Global.h"
#include "Bamocar.h"
#include "Motor.h"
#include "Pedals.h"
#include "ION_CAN.h"
#include "CAN.h"


uint16_t RTDS_Time = 0;

uint8_t START_Pushed = 0;
uint8_t STOP_Pushed = 0;

uint8_t Precharge_State = 0;

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
	
	// START_Pushed is set by an interrupt, so disable them while we read it
	__disable_irq();
	
	bool brake = getPedalValuef(PEDAL_BRAKE) > 0.05f;
	brake = true; // TODOTODOTODO! Fixme ;)
	bool start = (START_Pushed == 1);
	bool stop  = (STOP_Pushed == 1);
	
	START_Pushed = 0;
	STOP_Pushed = 0;
	__enable_irq();
	
	if(carState == PRECHARGE){
		carState = DISARMED; // TODO: Check precharge state
	}
	else if(carState == DISARMED)
	{
		LED_SetState(LED_GREEN, ENABLE);
		
		if(brake && start){
			
			/*
			Når du kobler til to motorkontrollere + bremsepedal:
			- Fjern brake = true over her
			- Les begge kontrollere i readErrors i MotorsPreArmCheck
			*/
			
			// Check torque-pedal
			if(getPedalValuef(PEDAL_TORQUE) > 0.0f){
				ReportStartupError(STARTUP_ERR_TORQUE);
				return;
			}
			
			// Motor pre-arm checks
			uint8_t preArm = MotorsPreArmCheck();
			if(preArm != 0){
			//	return;
			}
	
			// Start RTDS
			SetRTDS(ENABLE);
			RTDS_Time = 1000; // 1 sec
			
			// Switch state
			carState = ARMING;
			LED_SetState(LED_GREEN, DISABLE);
		}
	}
	else if(carState == ARMING){
		LED_SetState(LED_BLUE, ENABLE);
		if(RTDS_Time == 0)
		{
			// Stop RTDS
			SetRTDS(DISABLE);
			
			// Setup motorcontrollers
			if(MotorsEnable() != 0){
				carState = DISARMED;
				return;
			}
			
			// Arm car
			carState = ARMED;
			LED_SetState(LED_BLUE, DISABLE);
		}
	}
	else if(carState == ARMED){
		
		LED_SetState(LED_RED, ENABLE);
		
		MotorLoop();
		
		// Stop-button
		if(stop){
			MotorsDisable();
			
			carState = DISARMED;
			LED_SetState(LED_RED, DISABLE);
		}
	}
	
	/*if(brems && !stop){
		
		if(start){
		
			while(precharge == 0){
			
				CAN_TR(start);
				return 1;
					
			}
		}
	}	*/
}

void ReportStartupError(uint8_t errorCode){
	CANTx(CAN_ERR_STARTUP, 1, &errorCode);
}

void Startup_START_Pushed(void){
	START_Pushed = 1;
}

void Startup_STOP_Pushed(void){
	STOP_Pushed = 1;
}

//#include <stm32f4xx.h>
//#include "startup.h"

//uint8_t RTDS(void){
//	if( calibrateSensors()){
//		GPIO_SetBits(GPIOB, GPIO_Pin_13);
//		return 1;
//	}
//	else return 0;

//}

//uint8_t mcRun(void){
//	
//	if(RTDS() && pedalSensors[1] > 0xFFF/20){ // If RTDS has been sounded and brake pedal is engaged(>5%).
//	
//		GPIO_SetBits(GPIOB, GPIO_Pin_12); // Set RUN signal high.

//		return 1;
//	}
//	
//	else return 0;
//	
//}

//uint8_t startup(void){
//	
//	calibrateSensors();
//	
//	while(brems && !stop){
//		
//		if(start){
//		
//			while(precharge == 0){
//			
//				CAN_TR(start);
//				return 1;
//					
//			}
//		}
//	}	
//}
//	
//	
//	
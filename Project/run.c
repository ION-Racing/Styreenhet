#include <stm32f4xx.h>
#include "run.h"

uint8_t mcRun(void){
	
	if(RTDS() /*&& brake pedal > 0*/){ // If RTDS has been sounded and brake pedal is engaged.
	
		GPIO_SetBits(GPIOB, GPIO_Pin_12); // Set RUN signal high.

		return 1;
	}
	
	else return 0;
	
}

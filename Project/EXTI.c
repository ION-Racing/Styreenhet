#include "STM32f4xx.h"
#include "Global.h"

wheeld wheel;

void InitEXTI()
{
	/*Configure GPIOs as EXTI:
	PD9		: Wheelsensor 1
	PD10	: Wheelsensor 2
	*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//Configure syscfg
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
	
	EXTI_InitTypeDef EXTI_initstruct;
	EXTI_initstruct.EXTI_Line = EXTI_Line9 | EXTI_Line10;
	EXTI_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_initstruct);
}

#define TRIGGER1 0 
#define TRIGGER2 1 
#define debounceTime  25000 // t = debounceTime * 1us (= 10ms)
	
// Variables for wheelsensor interrupt	
uint32_t wp_temp1; // temporary delta time. 
uint32_t wp_temp2; // temporary delta time.

/*
Interrupt handlers for wheelsensor 1 and 2.
*/

void EXTI9_5_IRQHandler(void) 
{
	__disable_irq();

	// Wheelsensor interrupt action
    if (EXTI_GetITStatus(EXTI_Line9) != RESET) 	//Wheel sensor IT?
	{ 
		 if(wheel.state1 == TRIGGER1){						// First trigger?											
			wp_temp1 = TIM2->CNT; 									// Set reference time of first trigger
			wheel.state1 = TRIGGER2;								// Ready for state 2.
		 }
		 else {																		// Or second trigger..
			wheel.period1 = TIM2->CNT - wp_temp1;		// Calculate time difference of trigger 1 and 2.
			wp_temp1 = wheel.period1;								// Ready or state 1.												
		 }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
			 
	__enable_irq();			
}
 
/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void) 
{
	__disable_irq();

	if (EXTI_GetITStatus(EXTI_Line10) != RESET)		// Wheel sensor IT?
	{ 
		if(wheel.state2 == TRIGGER1){				// First trigger?											
			wp_temp2 = TIM2->CNT; 					// Set reference time of first trigger
			wheel.state2 = TRIGGER2;				// Ready for state 2.
		}
		else {										// Or second trigger..
			wheel.period2 = TIM2->CNT - wp_temp2;	// Calculate time difference of trigger 1 and 2.
			wp_temp2 = wheel.period2;
			//wheel.state2 = TRIGGER1;				// Ready or state 1.												
		}
        EXTI_ClearITPendingBit(EXTI_Line10);
    } 
	
	__enable_irq();			
}

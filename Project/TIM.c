/* TIM. Used for the Frequency_counter function to determine
	The wheel speed of the vehicle. */
	
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "TIM.h"
	
void InitTIM(void)
{
	/*
	Timer for interrupt purposes
	*/
	TIM_TimeBaseInitTypeDef TIM_structinit;
	//Enable clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_structinit.TIM_Prescaler = 84-1; // TIM2 = 1 Mhz
	TIM_structinit.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_structinit.TIM_Period = 0xFFFFFFFF; //
	TIM_structinit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_structinit);
	
	TIM_Cmd(TIM2,ENABLE);
	
	/*
	Timer for CAN-Watchdog
	*/
	//Enable clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_structinit.TIM_Prescaler = 8400-1;
	TIM_structinit.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_structinit.TIM_Period = 50000-1; //
	TIM_structinit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_structinit);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	// Timer for status-led
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_structinit.TIM_Prescaler = 84-1;
	TIM_structinit.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_structinit.TIM_Period = 50000-1; //
	TIM_structinit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_structinit);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);
	
	TIM_SetCompare2(TIM8, 10);
	
}
	
void	TIM4_IRQHandler(void)
{
		__disable_irq();
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	 {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		GPIOA->ODR ^= GPIO_Pin_6;
	 }
	 __enable_irq();
}

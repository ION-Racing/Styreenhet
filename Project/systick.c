#include "stm32f4xx.h"

const uint8_t COMPLETE = 0;
const uint8_t RESTART = 1;

void InitSystick(void) {
	
	SysTick_Config(SystemCoreClock / 1000); // Trigger SysTick every 1 ms
}


volatile uint16_t clk800msSPI = RESTART; 
volatile uint16_t clk100msSPI = RESTART; 
volatile uint32_t delay_ms_time = 0;

extern uint16_t RTDS_Time;

void SysTick_Handler(void)
{	
	if (clk800msSPI != COMPLETE)
	{
		clk800msSPI++;
		if(clk800msSPI >= 801) clk800msSPI = COMPLETE;
	}

	if (clk100msSPI != COMPLETE)
	{
		clk100msSPI++;
		if (clk100msSPI >= 101) clk100msSPI = COMPLETE;
	}
	
	// Ready-to-drive-sound
	if(RTDS_Time > 0)
		RTDS_Time--;
	
	// Delay
	delay_ms_time--;
}

void delay_ms(uint32_t time){
	delay_ms_time = time;
	while(delay_ms_time > 0);
}
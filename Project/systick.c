#include "stm32f4xx.h"

const uint8_t COMPLETE = 0;
const uint8_t RESTART = 1;

void InitSystick(void) {
	
	SysTick_Config(SystemCoreClock/1000); 		
	
}


volatile uint16_t clk800msSPI=RESTART ; 
volatile uint16_t clk100msSPI=RESTART ; 


void SysTick_Handler(void) {
	
if (clk800msSPI != COMPLETE)
	{
		clk800msSPI++;
		if (clk800msSPI >= 801) clk800msSPI = COMPLETE;
	}
	
if (clk100msSPI != COMPLETE)
	{
		clk100msSPI++;
		if (clk100msSPI >= 101) clk100msSPI = COMPLETE;
	}
}
#include "stm32f4xx.h"
#include "stm32f4xx_RCC.h"
#include "Periph_header.h"



/* Outputs 	: HSI, LSE, HSE or PLL clock on PA8.
 *					: HSE, PLL, SYSCLK, PLLI2S on PC9.
 * The prescaler can be from 1-5. 
 * The output frequency must not exceed 100Mhz.
 */
void MCO_Config(void){
	
	// Init struct 
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	
//	//Enable MCO1(PA8) as output
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
	//Enable MCO2(PC9) as output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_PinAFConfig(GPIOA, GPIO_Pin_8, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOC, GPIO_Pin_6, GPIO_AF_MCO);
	
	//RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
	RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_5);
	
	
	/* Does not work.
	//MCO 1 config
	RCC->CFGR &= ~(3<<29); // Output Sysclk on MC02.
	RCC->CFGR |= (7<<26);  // Set MC02 prescaler = 5.
*/

}

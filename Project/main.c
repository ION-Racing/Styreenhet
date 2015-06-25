#include "stm32f4xx.h"
#include "stm32f4xx_GPIO.h"
#include "GPIO.h"
#include "NVIC.h"
#include "CAN.h"
#include "ADC.h"
#include "SPI.h"


static void Delay(__IO uint32_t);
CanTxMsg msgTx;	  
CanRxMsg msgRx;

/*Gyro power-up:

	Wait 800 ms
	SPI_ReadData(0x00);
	SPI_ReadData(0x00);
*/

// Data-variabler
uint16_t pedalSensors[2];
			
int main(void)
{
	// Configure the system clock.
	// The system clock is 168Mhz.
	RCC_HSEConfig(RCC_HSE_ON); // ENABLE HSE (HSE = 8Mhz)
	while(!RCC_WaitForHSEStartUp());  // Wait for HSE to stabilize
	
	SystemCoreClockUpdate();
	RCC_PCLK1Config(RCC_HCLK_Div4); // Set APB1=42Mhz (168/4)

	
	// Initialize peripheral modules
	InitGPIO();
	InitNVIC();
	InitADC();
	InitCAN();
//	MCO_Config(); // Clock output
	
	
	
	//Enable a LED to show on status.
	GPIOC->ODR |= GPIO_Pin_8;
	
	/* Main code */
	while(1)
	{
		
		if(pedalSensors[0] > 0xF){
			GPIOB->ODR |= GPIO_Pin_14;				
		}
		else {
			GPIOB->ODR &= ~GPIO_Pin_14;
		}
		
	}
}



//--------------------------------------------------------------------
/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(__IO uint32_t nCount){
  while(nCount--){}
}

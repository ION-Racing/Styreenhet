#include "stm32f4xx.h"
#include "stm32f4xx_GPIO.h"
#include "GPIO.h"
#include "NVIC.h"
#include "CAN.h"
#include "ADC.h"

static void Delay(__IO uint32_t);
CanTxMsg msgTx;	  
CanRxMsg msgRx;
			
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
		Delay(0xFF);
		/*
		if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET){
		
		CAN_Receive(CAN1,CAN_FIFO0,&msgRx);
		if(msgRx.StdId == 0x1){
			GPIOC->ODR |= GPIO_Pin_6;
		}
	}
		*/
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

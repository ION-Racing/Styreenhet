#include "stm32f4xx.h"
#include "Periph_header.h"
#include "stm32f4xx_GPIO.h"

static void Delay(__IO uint32_t);
CanTxMsg msgTx;	  
CanRxMsg msgRx;
			
int main()
{
	
	/* Configure the system clock.
		The system clock is 168Mhz.
	*/
	RCC_HSEConfig(RCC_HSE_ON); //ENABLE HSE (HSE = 8Mhz)
while(!RCC_WaitForHSEStartUp()) 
{
	;// Wait for HSE to stabilize
}
SystemCoreClockUpdate(); //Must be called after changing Sysclk
RCC_PCLK1Config(RCC_HCLK_Div4); // Set APB1=42Mhz (168/4)

// Configure Perhipher modules
	GPIOC_Config();  
	NVIC_Config();
	CAN_Config();
	MCO_Config();
	/*Startup code */

//Enable a LED to show on status.
	GPIOC->ODR |= GPIO_Pin_8;
Init_RxMes(&msgRx);
/* Main code */

	while(1)
	{
		/*
		if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET){
		
		CAN_Receive(CAN1,CAN_FIFO0,&msgRx);
		if(msgRx.StdId == 0x1){
			GPIOC->ODR |= GPIO_Pin_6;
		}
	}*/
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

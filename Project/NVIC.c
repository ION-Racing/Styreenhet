#include "stm32f4xx.h"

/**
  * @brief  Configures the NVIC
  * @param  None
  * @retval None
  */
void InitNVIC(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	// CAN1 Receive Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

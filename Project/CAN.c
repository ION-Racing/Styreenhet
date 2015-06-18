#include "stm32f4xx_CAN.h"
#include "stm32f4xx_gpio.h"


void InitCAN(void){

	/* Setup CAN GPIO-pins
	
	PA11: CAN RX
	PA12: CAN TX
	*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 
	
	// Enable interrupts
	NVIC_EnableIRQ(CAN1_TX_IRQn);
    NVIC_EnableIRQ(CAN1_RX0_IRQn);

	// Setup CAN peripheral module
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	CAN_InitTypeDef CAN_InitStruct;
	CAN_StructInit(&CAN_InitStruct);
	
	CAN_InitStruct.CAN_NART = ENABLE; // No automatic retransmission
	
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStruct.CAN_SJW	= CAN_SJW_3tq;
	CAN_InitStruct.CAN_BS1	= CAN_BS1_4tq;
	CAN_InitStruct.CAN_BS2	= CAN_BS2_2tq;
	CAN_InitStruct.CAN_Prescaler = (42000000 / 7) / 500000;
	CAN_Init(CAN1, &CAN_InitStruct);
	
	// Configure interrupts
	CAN_ITConfig(CAN1, (CAN_IER_FMPIE0 |           /* enable FIFO 0 msg pending IRQ    */
						CAN_IER_TMEIE    ), ENABLE);       /* enable Transmit mbx empty IRQ    */
	
	uint8_t InitStatus = CAN_Init(CAN1, &CAN_InitStruct);


	// Setup filter
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CAN_FilterInitStruct.CAN_FilterNumber = 1;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterActivation = DISABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	// Start CAN
	CAN_OperatingModeRequest(CAN1, CAN_OperatingMode_Normal);

}

void CAN1_TX_IRQHandler (void) {

//  if (CAN1->TSR & CAN_TSR_RQCP0) {          /* request completed mbx 0        */
//    CAN1->TSR |= CAN_TSR_RQCP0;             /* reset request complete mbx 0   */
//    CAN1->IER &= ~CAN_IER_TMEIE;            /* disable  TME interrupt         */

//	  
//	  GPIOC->ODR ^= GPIO_Pin_7;
//	  
//	  //CAN_TxRdy[0] = 1; 
//  }
	
	if(CAN_GetITStatus(CAN1, CAN_IT_TME)){
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
		
		GPIOC->ODR ^= GPIO_Pin_8;
	}
}

void CAN1_RX0_IRQHandler (void) {

  if (CAN1->RF0R & CAN_RF0R_FMP0) {			    /* message pending ?              */
	 // CAN_rdMsg (1, &CAN_RxMsg[0]);           /* read the message               */

		//CAN_RxRdy[0] = 1;                       /*  set receive flag              */
	
	  GPIOC->ODR ^= GPIO_Pin_8;
  }
	
}
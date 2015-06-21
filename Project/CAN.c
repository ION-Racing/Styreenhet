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
	/*CAN_InitStruct.CAN_SJW	= CAN_SJW_3tq;
	CAN_InitStruct.CAN_BS1	= CAN_BS1_4tq;
	CAN_InitStruct.CAN_BS2	= CAN_BS2_2tq;*/
	CAN_InitStruct.CAN_Prescaler = (42000000 / 7) / 500000;
	
	
	// Configure interrupts
	CAN_ITConfig(CAN1, (CAN_IER_FMPIE0 |           /* enable FIFO 0 msg pending IRQ    */
						CAN_IER_TMEIE    ), ENABLE);       /* enable Transmit mbx empty IRQ    */
	
	uint8_t InitStatus = CAN_Init(CAN1, &CAN_InitStruct);

//	uint32_t brp  = (42000000 / 7) / 500000;         /* baudrate is set to 500k bit/s    */
//                                                                          
//  /* set BTR register so that sample point is at about 71% bit time from bit start */
//  /* TSEG1 = 4, TSEG2 = 2, SJW = 3 => 1 CAN bit = 7 TQ, sample at 71%      */
//  CAN1->BTR &= ~(((        0x03) << 24) | ((        0x07) << 20) | ((        0x0F) << 16) | (          0x3FF));
//  CAN1->BTR |=  ((((3-1) & 0x03) << 24) | (((2-1) & 0x07) << 20) | (((4-1) & 0x0F) << 16) | ((brp-1) & 0x3FF));
//	
//	CAN1->MCR &= ~CAN_MCR_INRQ; 

	// Setup filter
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CAN_FilterInitStruct.CAN_FilterNumber = 1;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterActivation = DISABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	// Start CAN
	if(CAN_OperatingModeRequest(CAN1, CAN_OperatingMode_Normal) != CAN_ModeStatus_Success){
		
		GPIOC->ODR = (GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8);
		
		//while(1);
	}
}

uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]) {
	
	CanTxMsg msg;	  
	msg.StdId 	= address;
	msg.IDE 	= CAN_Id_Standard;
	msg.RTR		= CAN_RTR_Data;
	msg.DLC		= length;
	
	uint8_t i = 0;
	for(i=0; i<length; i++){
		msg.Data[i] = data[i];
	}

	return CAN_Transmit(CAN1, &msg);
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

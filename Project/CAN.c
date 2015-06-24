#include "stm32f4xx_CAN.h"
#include "stm32f4xx_gpio.h"
#include "CAN.h"
#include "ION_CAN.h"
#include "Bamocar.h"



/* PRIVATE functions */
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage; //Used for testing

extern uint16_t pedalSensors[2];

extern CanRxMsg msgRx;

void InitCAN(void)
{
	// Setup CAN Pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Alternate functions
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	
	
	// Setup CAN-modules
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;

	/* CAN Baudrate = 500 KBps (CAN clocked at 42 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = (42000000 / 7) / 500000;
	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Transmit Structure preparation */
	TxMessage.StdId = 0x321;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 1;

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	
	Init_RxMes(&msgRx);
}

/* CAN Transmit */

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

/* CAN Receive */

void Init_RxMes(CanRxMsg *RxMessage)
{
	uint8_t ubCounter = 0;

	RxMessage->StdId = 0x00;
	RxMessage->ExtId = 0x00;
	RxMessage->IDE = CAN_ID_STD;
	RxMessage->DLC = 0;
	RxMessage->FMI = 0;
	for (ubCounter = 0; ubCounter < 8; ubCounter++)
	{
		RxMessage->Data[ubCounter] = 0x00;
	}
}

// CAN RX Interrupt
void CAN1_RX0_IRQHandler (void)
{
	if (CAN1->RF0R & CAN_RF0R_FMP0){
		
		/* Temp action for testing CAN */
		CAN_Receive(CAN1,CAN_FIFO0,&msgRx);

		
		if(msgRx.StdId == CAN_MSG_PEDAL_VALUES){
			pedalSensors[0] = msgRx.Data[2];
		}
		
		/* 		Code to read error messages from Motorcontrollers 		*/
		if(msgRx.StdId == MOTORCONTROLLER_RIGHT_RX_STDID && msgRx.Data[0] == 0x8F)
		{
			uint16_t error = msgRx.Data[1] + (msgRx.Data[2]<<8);
			readMotorControllerErrorR(error);
		}		
		
		if(msgRx.StdId == MOTORCONTROLLER_LEFT_RX_STDID && msgRx.Data[0] == 0x8F)
		{
			uint16_t error = msgRx.Data[1] + (msgRx.Data[2]<<8);
			readMotorControllerErrorL(error);
		}
		/************************************************************/

		/* 		Code to read Core Status from Motorcontrollers 		*/
		if(msgRx.StdId == MOTORCONTROLLER_LEFT_RX_STDID && msgRx.Data[0] == 0x40)
		{
			uint16_t core = msgRx.Data[1] + (msgRx.Data[2]<<8);
			readMotorControllerCoreL(core);
			
		}
		if(msgRx.StdId == MOTORCONTROLLER_LEFT_RX_STDID && msgRx.Data[0] == 0x40)
		{
			
		}		
		/************************************************************/		
	}
}

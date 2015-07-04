#include "stm32f4xx_CAN.h"
#include "stm32f4xx_gpio.h"
#include "CAN.h"
#include "ION_CAN.h"
#include "Bamocar.h"
#include "Motor.h"
#include "motorcontroller.h"
#include "startup.h"
#include "Pedals.h"

extern uint16_t pedalValues[2];
extern uint8_t Precharge_State;

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
	CAN_InitTypeDef CAN_InitStructure;
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
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
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

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/* CAN Transmit */

CanTxMsg txMsg;
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]) {
 
	txMsg.StdId 	= address;
	txMsg.IDE 	= CAN_Id_Standard;
	txMsg.RTR		= CAN_RTR_Data;
	txMsg.DLC		= length;
	
	uint8_t i = 0;
	for(i=0; i<length; i++){
		txMsg.Data[i] = data[i];
	}

	return CAN_Transmit(CAN1, &txMsg);
}

/* CAN Receive */
CanRxMsg msgRx;

// CAN RX Interrupt
void CAN1_RX0_IRQHandler (void){
	if (CAN1->RF0R & CAN_RF0R_FMP0)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &msgRx);		
		
		// Security
		if(msgRx.StdId == CAN_MSG_PRECHARGE){
			Precharge_State = msgRx.Data[0];
		}
		
		// Pedals
		else if(msgRx.StdId == CAN_MSG_PEDALS_STEERING){
			readEncoders((msgRx.Data[0]>>8)+msgRx.Data[1], (msgRx.Data[2]>>8)+msgRx.Data[3], msgRx.Data[4], msgRx.Data[5]);
		}
		
		// Motors
		else if(msgRx.StdId == CAN_MSG_MOTOR_LEFT_RX || msgRx.StdId == CAN_MSG_MOTOR_RIGHT_RX){
			
			uint8_t motorIdx;
			if(msgRx.StdId == CAN_MSG_MOTOR_LEFT_RX){
				motorIdx = 0;
			}
			else {
				motorIdx = 1;
			}
			
			uint32_t data = msgRx.Data[1] + (msgRx.Data[2] << 8); // 16-bit (DLC=4)
			
			if(msgRx.DLC >= 6)
				data += (msgRx.Data[3] << 16) + (msgRx.Data[4] << 24); // 32-bit (DLC=6)
			
			BamocarRx(motorIdx, msgRx.Data[0], data);
		}
		
		// Dashboard
		else if(msgRx.StdId == CAN_MSG_USER_START){
			Startup_START_Pushed();
		}
		else if(msgRx.StdId == CAN_MSG_USER_STOP){
			Startup_STOP_Pushed();
		}	
	}
}

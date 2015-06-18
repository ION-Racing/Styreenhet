#define HSI_VALUE	168000000

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "CAN.h"
#include "ION_CAN.h"

uint8_t teller = 0;

void Delay(__IO uint32_t nCount);
int main(void){
	
	RCC_HSEConfig(RCC_HSE_ON);
	
	SystemCoreClockUpdate();
	
	/*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	while(1){
		GPIOC->ODR ^= GPIO_Pin_6;
	}*/
	
	
	// Startup
	InitCAN();
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	uint8_t adr = 0;
	
  while (1){
	  
	  GPIOC->ODR ^= GPIO_Pin_6;
	  
	  Delay(0x00FFFF);
	  
    /*GPIO_SetBits(GPIOC, GPIO_Pin_6);
    Delay(0x0FFFFF);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    Delay(0x0FFFFF);*/
		
	 
	  CanTxMsg msg;	  
	  msg.StdId = 0x123;
	  msg.IDE 	= CAN_Id_Standard;
	  msg.RTR	= CAN_RTR_Data;
	  msg.DLC	= 2;
	  msg.Data[0] = 0x42;
	  msg.Data[1] = 0x43;
	  msg.Data[2] = 0;
	  msg.Data[3] = 0;
	  msg.Data[4] = 0;
	  msg.Data[5] = 0;
	  msg.Data[6] = 0;
	  msg.Data[7] = 0;
	  
	uint8_t box = CAN_Transmit(CAN1, &msg);
	  
	  uint8_t status = CAN_TransmitStatus(CAN1, box);
	if(status != CAN_TxStatus_Ok){
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
	}
	else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	
Delay(0x0FFFFF);		
	}
	  
		
  }
}

//--------------------------------------------------------------------
void Delay(__IO uint32_t nCount){
  while(nCount--){}
}

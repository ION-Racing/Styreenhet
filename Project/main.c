#define HSI_VALUE	168000000

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "CAN.h"
#include "ADC.h"
#include "LED.h"
#include "ION_CAN.h"

uint8_t teller = 0;

void Delay(__IO uint32_t nCount);
int main(void){
	
	RCC_HSEConfig(RCC_HSE_ON);
	SystemCoreClockUpdate();
	
	// Startup
	InitCAN();
	InitADC();
	InitLED();
	
	
	uint8_t adr = 0;
	
  while (1){
	  
	  GPIOC->ODR ^= GPIO_Pin_6;
	  
	  Delay(0x00FFFF);
	  
	  
	  uint8_t data[8] = {0x42, 0x43, 0x44};
	  CANTx(0x100, 3, data);
  }
}

//--------------------------------------------------------------------
void Delay(__IO uint32_t nCount){
  while(nCount--){}
}

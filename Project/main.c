#include "stm32f4xx.h"
#include "stm32f4xx_GPIO.h"
#include "GPIO.h"
#include "NVIC.h"
#include "CAN.h"
#include "ADC.h"
#include "SPI.h"
#include "systick.h"
#include "startup.h"


#define ID_UNIQUE_ADDRESS		0x1FFF7A10
#define TM_ID_GetUnique32(x)	((x >= 0 && x < 3) ? (*(uint32_t *) (ID_UNIQUE_ADDRESS + 4 * (x))) : 0)

static void Delay(__IO uint32_t);

// Data-variabler
uint16_t pedalSensors[2];

uint16_t gyrodata;

int main(void)
{
	
	
	
	// Check that you flashed to the correct microcontroller
	uint32_t chipId1 = TM_ID_GetUnique32(0);
	uint32_t chipId2 = TM_ID_GetUnique32(1);
	uint32_t chipId3 = TM_ID_GetUnique32(2);
	if(chipId1 != 0x003C0045 || chipId2 != 0x30345117 || chipId3 != 0x37333838){
		while(1);
	}
	
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
	InitSystick();
	InitSPI();
//	MCO_Config(); // Clock output
	
	startup();
	// Startup finished LED
	LED_SetState(LED_GREEN, ENABLE);
	
	
	/* Start communication with AKS/GYRO.
	Must happen after approximately 800ms
	after startup*/
	SPIstartCommunication();
	
	if(startup()){
	 RTDS();
		mcRun();
	}
	
	while(1)
	{
		
		if(clk100msSPI == COMPLETE ) 
		{
			gyrodata = SPI_ReadData(0x00);
			clk100msSPI = RESTART;
		}
		
		// Brakelight
		if(pedalSensors[1] > 0xFFF / 20){ // Brake > 5%: (1/0.05) = 20
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

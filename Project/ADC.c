#include "stm32f4xx_gpio.h"

//#define BUFFERSIZE 448  // Setter opp st¯rrelsen pÂ ADC array.
#define BUFFERSIZE  (4+4)*7

// ADC-values
__IO uint16_t ADCDualConvertedValues[BUFFERSIZE];

uint16_t rawSensorValues[7];

void InitADC(void){
	
	/*
		Sensor			Pin		Pin function	 Sensor function
		--------------------------------------------------------
		Styresnekke		PA1		ADC123_IN1		 Steering
		Pedalsensor 1	PA2		ADC123_IN2		 Throttle 1
		Pedalsensor 2	PA3		ADC123_IN3		~Throttle 1
		Pedalsensor 3	PA4		ADC12_IN4		 Throttle 2
		Pedalsensor 4	PA5		ADC12_IN5		~Throttle 2
		Pedalsensor 5	PA6		ADC12_IN6		 Brake
		Pedalsensor 6	PA7		ADC12_IN7		~Brake
	
		Two ADC-modules are used, ADC1 for channel 2, 3, 4 & 1, and ADC2 for channel 5, 6, 7.
		After conversion the values are transferred to ADCDualConvertedValues via DMA in the following order (Dual ADC Mode, DMA Mode 1):
		CH2, CH5, CH3, CH6, CH4, CH7, CH1
	*/
	
	// Setup ADC-pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	
	
	// Configure NVIC
	NVIC_InitTypeDef NVIC_InitStructure;

	// DMA Stream 0 IRQ
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	
	// Setup DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_0;
	DMA_InitStructure.DMA_Memory0BaseAddr		= (uint32_t)&ADCDualConvertedValues[0];
	DMA_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t)0x40012308; // CDR_ADDRESS; Packed ADC1, ADC2
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize 			= BUFFERSIZE; // Count of 16-bit words
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;  // Setter verdiene i ADCDualConvertedValues i loop.
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	/* Enable DMA Stream Half / Transfer Complete interrupt */
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); // | DMA_IT_HT, ENABLE); // Interrupt for Transfer complete. (Buffersize / No of ADC channels = no of samples before interrupt)

	/* DMA2_Stream0 enable */
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	
	
	
	// Setup ADC-module (ADC1 & ADC2)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	/* ADC Common Init */
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode 				= ADC_DualMode_RegSimult; //Sampler to og to signaler samtidig
	ADC_CommonInitStructure.ADC_Prescaler 			= ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode 		= ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution			= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode 			= ENABLE;  // Multikanal per ADC
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_T2_TRGO; // Trigger conversion with TIM2
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion 		= 4;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_InitStructure.ADC_NbrOfConversion 		= 4;
	ADC_Init(ADC2, &ADC_InitStructure); // Mirror on ADC2

	/* ADC1 regular channel 11 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles); // PA2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_480Cycles); // PA3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_480Cycles); // PA4
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 4, ADC_SampleTime_480Cycles); // PA1
	
	/* ADC2 regular channel 12 configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_480Cycles); // PA5
	ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 2, ADC_SampleTime_480Cycles); // PA6
	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 3, ADC_SampleTime_480Cycles); // PA7
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 4, ADC_SampleTime_480Cycles); // PB0 (for equal lengths on both channels)

	/* Enable DMA request after last transfer (Multi-ADC mode)  */
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	// Enable ADC
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	
	
	
	
	// Setup TIM2 for ADC conversion triggering
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = (84000000 / 1280) - 1; // 2 KHz, from 84 MHz TIM5CLK (ie APB1 = HCLK/4, TIM5CLK = HCLK/2)
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); // ADC_ExternalTrigConv_T2_TRGO

	TIM_Cmd(TIM2, ENABLE);
}



/* DMA Interrupt */
void DMA2_Stream0_IRQHandler(void)
{ 
	// DMA Stream Transfer Complete
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)){
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		
		rawSensorValues[0] = ADCDualConvertedValues[6];
		rawSensorValues[1] = ADCDualConvertedValues[0];
		rawSensorValues[2] = ADCDualConvertedValues[2];
		rawSensorValues[3] = ADCDualConvertedValues[4];
		rawSensorValues[4] = ADCDualConvertedValues[1];
		rawSensorValues[5] = ADCDualConvertedValues[3];
		rawSensorValues[6] = ADCDualConvertedValues[5];
		
		//movingAverage(ADCDualConvertedValues);
	}
}




#include <stm32f4xx.h>
#include <stm32f4xx_spi.h>
#include <stdio.h>
#include "SPI.h"

// http://www.lxtronic.com/index.php/basic-spi-simple-read-write 
// http://www.mouser.com/pdfdocs/tn92_spi_communication_with_scc1300_rev_11.PDF

void SPI_Configuration(void){
	
	/*SPI interface setup*/	
	
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_16b; 
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low; 		// (0x0000)
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;  // (0x0000)
	 
	SPI_Init(SPI1, &SPI_InitTypeDefStruct);

	SPI_Cmd(SPI1, ENABLE);

}	

uint8_t CalcParity(uint16_t Data){
	
	uint8_t NoOfOnes = 0;

	while(Data != 0){

		NoOfOnes++;
		Data &= (Data - 1); // Loop will execute once for each bit of Data set
		}
	
	// If NoOfOnes is odd, least significant bit will be 1
	return (~NoOfOnes & 1);
} 

uint16_t SPI_ReadData(uint8_t address){

	uint16_t convertedGyro;
	uint16_t ui16Address;
	
	// Build address transfer frame
	ui16Address = address;
	ui16Address <<= 3; // Address to be shifted left by 3
	ui16Address += CalcParity(ui16Address);	//Add parity
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);			// Select gyroscope 
	 
	ui16Address = 0x80 | ui16Address; 			//0x80 = 10000000. Setting the MSB high tells the sensor to read and not to write.
	
	//status = SPI1->DR;											
	SPI_I2S_ReceiveData(SPI1);							// Read RX buffer just to clear interrupt flag.
	
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
	SPI_I2S_SendData(SPI1, ui16Address);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1); 							//Clear RXNE bit
	 
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
	SPI_I2S_SendData(SPI1, 0x01); 					//Dummy byte (with correct parity) to generate clock
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	 
	GPIO_SetBits(GPIOB, GPIO_Pin_7);				// Deselect gyroscope
	
	//Convert data from gyro rate register to angular speed (degrees per second):
	convertedGyro = SPI_I2S_ReceiveData(SPI1);
	convertedGyro >>= 2;		//Bits 1:0 do not contain angular speed data
	convertedGyro /= 18;		//The sensor's sensitivity is 18 LSB/dps
	 
	return convertedGyro;
}

void SPI_WriteData(uint8_t address, uint16_t data){
 
	//uint16_t status;
	uint16_t ui16Address;
	

	ui16Address = address;
	ui16Address <<= 3;
	ui16Address |= 0x04; // Set RW bit
	ui16Address += CalcParity(ui16Address); // Add parity bit 
	
	// Add parity to data
	data <<= 1;
	data += CalcParity(data); 

	GPIO_ResetBits(GPIOB, GPIO_Pin_7); 	// Select gyroscope
		
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
	SPI_I2S_SendData(SPI1, address);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);										

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOB, GPIO_Pin_7);		// Deselect gyrosope
	//return status;

}

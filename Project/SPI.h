#ifndef __spi_h
#define __spi_h

#include <stm32f4xx.h>


void SPI_WriteData(uint8_t address, uint16_t data);
uint16_t SPI_ReadData(uint8_t address);

#endif

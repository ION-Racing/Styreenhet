#include "stm32f4xx.h"

#ifndef __CAN_H
#define __CAN_H

#define CAN_RX_PIN GPIO_Pin_11
#define CAN_TX_PIN GPIO_Pin_12


void InitCAN(void);
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]);
void Init_RxMes(CanRxMsg *RxMessage);

#endif

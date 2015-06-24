#include "stm32f4xx.h"

#ifndef __CAN_H
#define __CAN_H

#define CAN_RX_PIN GPIO_Pin_11
#define CAN_TX_PIN GPIO_Pin_12
#define MOTORCONTROLLER_LEFT_RX_STDID 0x181
#define MOTORCONTROLLER_LEFT_TX_STDID 0x201
#define MOTORCONTROLLER_RIGHT_RX_STDID 0x182
#define MOTORCONTROLLER_RIGHT_TX_STDID 0x202

void InitCAN(void);
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]);
void Init_RxMes(CanRxMsg *RxMessage);

#endif

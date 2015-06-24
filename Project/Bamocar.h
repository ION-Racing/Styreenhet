#include "stm32f4xx.h"

#ifndef __MOTORCONTROLLER_H
#define __MOTORCONTROLLER_H

uint16_t motorcontrollerError;
void enableMotorcontrollers(void);
void disableMotorcontrollers(void);
void setTorque(int16_t hoyre, int16_t venstre);
void setRPM(int16_t hoyre, int16_t venstre);
uint16_t readRPMSetpointR(void);
uint16_t readRPMSetpointL(void);
uint16_t readRPMActualR(void);
uint16_t readRPMActualL(void);
uint16_t readTorqueSetpointR(void);
uint16_t readTorqueSetpointL(void);
uint16_t readMotorControllerStatusR(void);
uint16_t readMotorControllerStatusL(void);
void readMotorControllerErrorR(uint16_t errorMessage);
void readMotorControllerErrorL(uint16_t errorMessage);
void readMotorControllerCoreR(uint16_t coreStatus);
void readMotorControllerCoreL(uint16_t coreStatus);
#endif

#include "stm32f4xx.h"

//uint16_t motorcontrollerError;

void enableMotorcontrollers(void);
void disableMotorcontrollers(void);
void setTorque(int16_t hoyre, int16_t venstre);
void setRPM(int16_t hoyre, int16_t venstre);
uint16_t readRPMSetpointR(void);
uint16_t readRPMSetpointL(void);
void readRPMActualR(void);
void readRPMActualL(void);
uint16_t readTorqueSetpointR(void);
uint16_t readTorqueSetpointL(void);
uint16_t readMotorControllerStatusR(void);
uint16_t readMotorControllerStatusL(void);
void readMotorControllerErrorR(uint16_t errorMessage);
void readMotorControllerErrorL(uint16_t errorMessage);
void readMotorControllerCoreR(uint32_t coreStatus);
void readMotorControllerCoreL(uint32_t coreStatus);

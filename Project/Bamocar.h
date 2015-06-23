

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

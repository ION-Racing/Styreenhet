#include "stm32f4xx.h"


#define GRADIENT_AS 0.0526		// Stigningstallet til antispinnfunksjonen.
#define MAX_SPIN 115 					// Will allow 15 percent spin
#define SPIN 1






uint16_t tractionControl(uint16_t torque, uint16_t front, uint16_t rear);
void torqueVector(uint16_t velocity, uint8_t steeringDirection, uint8_t steeringDegrees);
void readRPM(uint8_t position, uint16_t left, uint16_t right);
void readEncoders(uint8_t torque, uint16_t brake, uint8_t steeringDir, uint8_t steeringDeg);



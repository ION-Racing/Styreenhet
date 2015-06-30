#include "stm32f4xx.h"
#include "Pedals.h"

extern uint16_t pedalValues[2];

void InitPedals(void){
	
	pedalValues[PEDAL_TORQUE] = 0;
	pedalValues[PEDAL_BRAKE] = 0;
	
}

void setPedalValues(uint16_t torque, uint16_t brake){
	pedalValues[PEDAL_TORQUE] = torque;
	pedalValues[PEDAL_BRAKE]  = brake;
}

uint16_t getPedalValue(uint8_t pedal){
	return pedalValues[pedal];
}

float getPedalValuef(uint8_t pedal){
	return (float)pedalValues[pedal] / (float)0xFFF;
}

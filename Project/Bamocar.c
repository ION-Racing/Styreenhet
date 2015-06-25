/*Functions for communicating with Bamocar D3 over CAN-BUS*/

#include "stm32f4xx.h"
#include "Bamocar.h"
#include "CAN.h"

uint8_t motorControllerRightDisabled, motorControllerLeftDisabled;

void enableMotorcontrollers(void)
{
	uint8_t enable[3] = {0x51, 0x00, 0x00};
	uint8_t requestCoreStatus[3] = {0x3d, 0x40, 0x00};
	
	while (motorControllerRightDisabled && motorControllerLeftDisabled)
	{		
		if (motorControllerRightDisabled){
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, enable); 
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, requestCoreStatus); 			//sindre test
		}
		if (motorControllerLeftDisabled){
			CANTx(MOTORCONTROLLER_LEFT_TX_STDID,  3, enable);
			CANTx(MOTORCONTROLLER_LEFT_TX_STDID,  3, requestCoreStatus);
		}
	}	
}

void disableMotorcontrollers(void)
{
	uint8_t disable[3] = {0x51, 0x04, 0x00};
	uint8_t requestCoreStatus[3] = {0x3d, 0x40, 0x00};

	while (!(motorControllerRightDisabled && motorControllerLeftDisabled))
	{		
		if (!motorControllerRightDisabled){
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, disable);
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, requestCoreStatus); 			
		}
		if (!motorControllerLeftDisabled){
			CANTx(MOTORCONTROLLER_LEFT_TX_STDID,  3, disable);
			CANTx(MOTORCONTROLLER_LEFT_TX_STDID,  3, requestCoreStatus); 						
		}
	}
}

void setTorque(int16_t hoyre, int16_t venstre)
{
	uint8_t torqueR[3] = {0x90, hoyre,   hoyre>>8};
	uint8_t torqueL[3] = {0x90, venstre, venstre>>8};
	CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, torqueR);
	CANTx(MOTORCONTROLLER_LEFT_TX_STDID, 3, torqueL);	
}

void setRPM(int16_t hoyre, int16_t venstre)
{
	uint8_t RPM_R[3] = {0x31, hoyre,   hoyre>>8};
	uint8_t RPM_L[3] = {0x31, venstre, venstre>>8};
	CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, RPM_R);
	CANTx(MOTORCONTROLLER_LEFT_TX_STDID, 3, RPM_L);		
}

uint16_t readRPMSetpointR(void)
{
	
}

uint16_t readRPMSetpointL(void)
{
	
}
uint16_t readRPMActualR(void)
{
	
}

uint16_t readRPMActualL(void)
{
	
}


uint16_t readTorqueSetpointR(void)
{
	
}

uint16_t readTorqueSetpointL(void)
{
	
}

uint16_t readMotorControllerStatusR(void)
{
	
}

uint16_t readMotorControllerStatusL(void)
{
	
}


void readMotorControllerErrorR(uint16_t errorMessage)
{
	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((errorMessage >> i) & 0x01)
		{
			/* TODO: 
			switch (i)
			{
				case 0:
				
			}
			*/
		}
	}
}

void readMotorControllerErrorL(uint16_t errorMessage)
{
	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((errorMessage >> i) & 0x01)
		{
			/* TODO: 
			switch (i)
			{
				case 0:
				
			}
			*/
		}
	}
}

void readMotorControllerCoreR(uint16_t coreStatus)
{
	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((coreStatus >> i) & 0x01)
		{
			/* TODO:  COMPLETE SWITCH CASE */
			switch (i)
			{
				case 0:
					motorControllerRightDisabled = 1;			
			}
		}
		else
		{
			/* TODO:  COMPLETE SWITCH CASE */
			switch (i)
			{
				case 0:
					motorControllerRightDisabled = 0;				
			}
		}
	}
}

void readMotorControllerCoreL(uint16_t coreStatus)
{
	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((coreStatus >> i) & 0x01)
		{
			/* TODO:  COMPLETE SWITCH CASE */
			switch (i)
			{
				case 0:
					motorControllerLeftDisabled = 1;	
			}
		}
		else
		{
			/* TODO:  COMPLETE SWITCH CASE */
			switch (i)
			{
				case 0:
					motorControllerLeftDisabled = 0;
				
			}
		}	
	}	
}

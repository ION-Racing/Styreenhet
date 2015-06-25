/*Functions for communicating with Bamocar D3 over CAN-BUS*/

#include "stm32f4xx.h"
#include "Bamocar.h"
#include "CAN.h"
#include "ION_CAN.h"

uint8_t motorControllerRightDisabled, motorControllerLeftDisabled;
static void Delay(__IO uint32_t);

void enableMotorcontrollers(void)
{
	uint8_t enable[3] = {0x51, 0x00, 0x00};
	uint8_t requestCoreStatus[3] = {0x3d, 0x40, 0x00};
	
	while (motorControllerRightDisabled && motorControllerLeftDisabled)
	{		
		if (motorControllerRightDisabled){
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, enable); 
			CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, requestCoreStatus); 			
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
void readRPMActualR(void)
{
	uint8_t requestRPM[3] = {0x3d, 0x30, RPM_INTERVAL_REQUEST};
	CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, requestRPM);
	
}

void readRPMActualL(void)
{
	uint8_t requestRPM[3] = {0x3d, 0x30, RPM_INTERVAL_REQUEST};
	CANTx(MOTORCONTROLLER_LEFT_TX_STDID, 3, requestRPM);	
}


uint16_t readTorqueSetpointR(void)
{
	uint8_t requestTorqueSetPoint[3] = {0x3d, 0x90, 0x00};
	CANTx(MOTORCONTROLLER_RIGHT_TX_STDID, 3, requestTorqueSetPoint);	
}

uint16_t readTorqueSetpointL(void)
{
	uint8_t requestTorqueSetPoint[3] = {0x3d, 0x90, 0x00};
	CANTx(MOTORCONTROLLER_LEFT_TX_STDID, 3, requestTorqueSetPoint);		
}


void readMotorControllerErrorR(uint16_t errorMessage)
{
	uint8_t motorcontrollerRIGHT[1] = {MOTORCONTROLLER_RIGHT};
		
	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((errorMessage >> i) & 0x01)
		{
			/*
							ERROR MESSAGE												STDID		DATA[0]				DESCRIPTION
																													0x00 / 0x01		0x00 = RIGHT CONTROLLER, 0x01 = LEFT CONTROLLER
			**************************************************
			#define	MOTORCONTROLLER_ERROR_BADPARAS 			0x074	
			#define	MOTORCONTROLLER_ERROR_POWER_FAULT  	0x073
			#define	MOTORCONTROLLER_ERROR_RFE_FAULT  		0x072
			#define	MOTORCONTROLLER_ERROR_BUS_TIMEOUT  	0x071
			#define	MOTORCONTROLLER_ERROR_FEEDBACK	  	0x070
			#define	MOTORCONTROLLER_ERROR_POWERVOLTAGE 	0x06F
			#define	MOTORCONTROLLER_ERROR_MOTORTEMP 		0x06E
			#define	MOTORCONTROLLER_ERROR_DEVICETEMP 		0x06D
			#define	MOTORCONTROLLER_ERROR_OVERVOLTAGE 	0x06C
			#define	MOTORCONTROLLER_ERROR_I_PEAK 			 	0x06B
			#define	MOTORCONTROLLER_ERROR_RACEAWAY 		 	0x06A
			#define	MOTORCONTROLLER_ERROR_CPU_ERROR 		0x069
			#define	MOTORCONTROLLER_ERROR_BALLAST 			0x068
			***************************************************
			*/
			switch (i)
			{
				case 0:
					CANTx(MOTORCONTROLLER_ERROR_BADPARAS, 		1, motorcontrollerRIGHT);
				case 1:
					CANTx(MOTORCONTROLLER_ERROR_POWER_FAULT, 	1, motorcontrollerRIGHT);
				case 2:
					CANTx(MOTORCONTROLLER_ERROR_RFE_FAULT, 		1, motorcontrollerRIGHT);
				case 3:
					CANTx(MOTORCONTROLLER_ERROR_BUS_TIMEOUT, 	1, motorcontrollerRIGHT);
				case 4:
					CANTx(MOTORCONTROLLER_ERROR_FEEDBACK, 		1, motorcontrollerRIGHT);
				case 5:
					CANTx(MOTORCONTROLLER_ERROR_POWERVOLTAGE, 1, motorcontrollerRIGHT);
				case 6:
					CANTx(MOTORCONTROLLER_ERROR_MOTORTEMP, 		1, motorcontrollerRIGHT);
				case 7:
					CANTx(MOTORCONTROLLER_ERROR_DEVICETEMP, 	1, motorcontrollerRIGHT);
				case 8:
					CANTx(MOTORCONTROLLER_ERROR_OVERVOLTAGE, 	1, motorcontrollerRIGHT);
				case 9:
					CANTx(MOTORCONTROLLER_ERROR_I_PEAK, 			1, motorcontrollerRIGHT);
				case 10:
					CANTx(MOTORCONTROLLER_ERROR_RACEAWAY, 		1, motorcontrollerRIGHT);
				case 14:
					CANTx(MOTORCONTROLLER_ERROR_CPU_ERROR, 		1, motorcontrollerRIGHT);
				case 15:
					CANTx(MOTORCONTROLLER_ERROR_BALLAST, 			1, motorcontrollerRIGHT);	
			}
			Delay(168000);
		}
	}
}

void readMotorControllerErrorL(uint16_t errorMessage)
{
	uint8_t motorcontrollerLEFT[1] = {MOTORCONTROLLER_LEFT};

	for (uint8_t i = 0; i < 16; i++) 
	{
		if ((errorMessage >> i) & 0x01)
		{
			/*
							ERROR MESSAGE												STDID		DATA[0]				DESCRIPTION
																													0x00 / 0x01		0x00 = RIGHT CONTROLLER, 0x01 = LEFT CONTROLLER
			**************************************************
			#define	MOTORCONTROLLER_ERROR_BADPARAS 			0x074	
			#define	MOTORCONTROLLER_ERROR_POWER_FAULT  	0x073
			#define	MOTORCONTROLLER_ERROR_RFE_FAULT  		0x072
			#define	MOTORCONTROLLER_ERROR_BUS_TIMEOUT  	0x071
			#define	MOTORCONTROLLER_ERROR_FEEDBACK	  	0x070
			#define	MOTORCONTROLLER_ERROR_POWERVOLTAGE 	0x06F
			#define	MOTORCONTROLLER_ERROR_MOTORTEMP 		0x06E
			#define	MOTORCONTROLLER_ERROR_DEVICETEMP 		0x06D
			#define	MOTORCONTROLLER_ERROR_OVERVOLTAGE 	0x06C
			#define	MOTORCONTROLLER_ERROR_I_PEAK 			 	0x06B
			#define	MOTORCONTROLLER_ERROR_RACEAWAY 		 	0x06A
			#define	MOTORCONTROLLER_ERROR_CPU_ERROR 		0x069
			#define	MOTORCONTROLLER_ERROR_BALLAST 			0x068
			***************************************************
			*/
			switch (i)
			{
				case 0:
					CANTx(MOTORCONTROLLER_ERROR_BADPARAS, 		1, motorcontrollerLEFT);
				case 1:
					CANTx(MOTORCONTROLLER_ERROR_POWER_FAULT, 	1, motorcontrollerLEFT);
				case 2:
					CANTx(MOTORCONTROLLER_ERROR_RFE_FAULT, 		1, motorcontrollerLEFT);
				case 3:
					CANTx(MOTORCONTROLLER_ERROR_BUS_TIMEOUT, 	1, motorcontrollerLEFT);
				case 4:
					CANTx(MOTORCONTROLLER_ERROR_FEEDBACK, 		1, motorcontrollerLEFT);
				case 5:
					CANTx(MOTORCONTROLLER_ERROR_POWERVOLTAGE, 1, motorcontrollerLEFT);
				case 6:
					CANTx(MOTORCONTROLLER_ERROR_MOTORTEMP, 		1, motorcontrollerLEFT);
				case 7:
					CANTx(MOTORCONTROLLER_ERROR_DEVICETEMP, 	1, motorcontrollerLEFT);
				case 8:
					CANTx(MOTORCONTROLLER_ERROR_OVERVOLTAGE, 	1, motorcontrollerLEFT);
				case 9:
					CANTx(MOTORCONTROLLER_ERROR_I_PEAK, 			1, motorcontrollerLEFT);
				case 10:
					CANTx(MOTORCONTROLLER_ERROR_RACEAWAY, 		1, motorcontrollerLEFT);
				case 14:
					CANTx(MOTORCONTROLLER_ERROR_CPU_ERROR, 		1, motorcontrollerLEFT);
				case 15:
					CANTx(MOTORCONTROLLER_ERROR_BALLAST, 			1, motorcontrollerLEFT);				}
		}
				Delay(168000);

	}
}

void readMotorControllerCoreR(uint32_t coreStatus)
{
	uint8_t stat[2] = {0x00, 0x00};
	for (uint16_t i = MOTORCONTROLLER_STATUS_ENABLE; i < MOTORCONTROLLER_STATUS_ENABLE+32; i++) 
	{
		stat[1]=((coreStatus>>(i-MOTORCONTROLLER_STATUS_ENABLE))&0x01);
		CANTx(i,2,stat);
		Delay(168000);

	}
						/*
			#define	MOTORCONTROLLER_STATUS_ENABLE		 							0x300	
			#define	MOTORCONTROLLER_STATUS_DRIVE_STOPPED					0x301	
			#define	MOTORCONTROLLER_STATUS_OUTPUT_LIM+_ACTIVE			0x302	
			#define	MOTORCONTROLLER_STATUS_OUTPUT_LIM-_ACTIVE			0x303	
			#define	MOTORCONTROLLER_STATUS_VACANT				 					0x304	
			#define	MOTORCONTROLLER_STATUS_LIMIT_TO_CONT_CURRENT	0x305	
			#define	MOTORCONTROLLER_STATUS_P_I										0x306	// Position control. End position – current(I) controller, directly coupled mode
			#define	MOTORCONTROLLER_STATUS_P_N										0x307	
										
			#define	MOTORCONTROLLER_STATUS_SPEED_CONTROL					0x308	
			#define	MOTORCONTROLLER_STATUS_SPEED_LOW		 					0x309	// Speed inferior to 0.1%
			#define	MOTORCONTROLLER_STATUS_REF_ACTIVE		 					0x30A	
			#define	MOTORCONTROLLER_STATUS_CALIB_TRAVEL		 				0x30B	
			#define	MOTORCONTROLLER_STATUS_CALIB_REF_POS		 			0x30C	
			#define	MOTORCONTROLLER_STATUS_POS_WITHIN_TOL		 			0x30D	
			#define	MOTORCONTROLLER_STATUS_DRIVE_READY		 				0x30E	
			#define	MOTORCONTROLLER_STATUS_BRAKE_ACTIVE		 				0x30F	
										
			#define	MOTORCONTROLLER_STATUS_SIGNMAG		 						0x310	
			#define	MOTORCONTROLLER_STATUS_SPEED_LIM		 					0x311	
			#define	MOTORCONTROLLER_STATUS_SPEED_LIM+		 					0x312	
			#define	MOTORCONTROLLER_STATUS_SPEED_LIM-		 					0x313	
			#define	MOTORCONTROLLER_STATUS_IRD_DIG		 						0x314	
			#define	MOTORCONTROLLER_STATUS_IUSE_RCHD		 					0x315	
			#define	MOTORCONTROLLER_STATUS_IRD_N		 							0x316	
			#define	MOTORCONTROLLER_STATUS_IRD_TI		 							0x317	
										
			#define	MOTORCONTROLLER_STATUS_IRD_TIR		 						0x318	
			#define	MOTORCONTROLLER_STATUS_2HZ		 								0x319	
			#define	MOTORCONTROLLER_STATUS_IRD_TM		 							0x31A	
			#define	MOTORCONTROLLER_STATUS_IRD_ANA		 						0x31B	
			#define	MOTORCONTROLLER_STATUS_IWCNS		 							0x31C	
			#define	MOTORCONTROLLER_STATUS_RFE_PULS		 						0x31D	
			#define	MOTORCONTROLLER_STATUS_VACANT		 							0x31E	
			#define	MOTORCONTROLLER_STATUS_HANDWHL		 						0x31F	
			*/
}

void readMotorControllerCoreL(uint32_t coreStatus)
{
	uint8_t stat[2] = {0x01, 0x00};
	for (uint16_t i = MOTORCONTROLLER_STATUS_ENABLE; i < MOTORCONTROLLER_STATUS_ENABLE+32; i++) 
	{
		stat[1]=((coreStatus>>(i-MOTORCONTROLLER_STATUS_ENABLE))&0x01);
		CANTx(i,2,stat);
		Delay(168000);
		
	}
}

//--------------------------------------------------------------------
/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(__IO uint32_t nCount){
  while(nCount--){}
}

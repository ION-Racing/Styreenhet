// ION CAN Messages v5 - 2015-07-02

#define	CAN_ERR_HV_SHUTDOWN								0x000
#define	CAN_ERR_IMD										0x001
#define	CAN_ERR_BMS										0x002
#define	CAN_ERR_OVERTEMP								0x003
#define	CAN_MSG_PRECHARGE								0x010
#define	CAN_MSG_CURRENT_SENSOR							0x011
#define	CAN_MSG_CURRENT_SENSOR_CALIBRATE				0x012
#define	CAN_MSG_CURRENT SENSOR_CALIBRATION_COMPLETE		0x013
#define	CAN_MSG_HV_SENSOR								0x014
#define	CAN_MSG_LV_SENSOR								0x015
#define	CAN_MSG_POWER									0x016

#define	CAN_MSG_MOTOR_LEFT_RX							0x181
#define	CAN_MSG_MOTOR_RIGHT_RX							0x182
#define	CAN_MSG_MOTOR_LEFT_TX							0x201
#define	CAN_MSG_MOTOR_RIGHT_TX							0x202

#define	CAN_ERR_PEDALS_IMPLAUSIBILITY					0x300
#define	CAN_MSG_PEDALS									0x310
#define	CAN_MSG_STEERING								0x311
#define	CAN_MSG_PEDALS_CALIBRATE						0x320
#define	CAN_MSG_PEDALS_CALIBRATION_COMPLETE				0x321

#define	CAN_MSG_USER_START								0x400
#define	CAN_MSG_USER_STOP								0x401
#define	CAN_MSG_WHEEL_RPM_FRONT							0x450

#define	CAN_MSG_BMS_PRODUCT								0x620
#define	CAN_MSG_BMS_REVISION							0x621
#define	CAN_MSG_BMS_FLAGS								0x622
#define	CAN_MSG_BMS_VOLTAGE								0x623
#define	CAN_MSG_BMS_CURRENT								0x624
#define	CAN_MSG_BMS_ENERGY								0x625
#define	CAN_MSG_BMS_CHARGE								0x626
#define	CAN_MSG_BMS_TEMP								0x627
#define	CAN_MSG_BMS_RESISTANCE							0x628








#define MOTORCONTROLLER_LEFT_RX_STDID 0x181
#define MOTORCONTROLLER_LEFT_TX_STDID 0x201
#define MOTORCONTROLLER_RIGHT_RX_STDID 0x182
#define MOTORCONTROLLER_RIGHT_TX_STDID 0x202

#define	CAN_MSG_WHEEL_RPM					0x400





#define RPM_INTERVAL_REQUEST	0x64					// HEX/DECIMAL NUMBER FOR n MILISECONDS

#define	MOTORCONTROLLER_RIGHT 0x00
#define	MOTORCONTROLLER_LEFT  0x01
/*
				ERROR MESSAGE												STDID		DATA[0]				DESCRIPTION
																										0x00 / 0x01		0x00 = RIGHT CONTROLLER, 0x01 = LEFT CONTROLLER
*/
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

/*
				STATUS MESSAGE																STDID		DATA[0]				DESCRIPTION									DATA[1] 		DESCRIPTION
																															0x00 /				0x00 = RIGHT CONTROLLER, 		0x01 /			0x01 = HIGH / ON 
																															0x01					0x01 = LEFT CONTROLLER			0x00				0x00 = LOW  / OFF
*/
#define	MOTORCONTROLLER_STATUS_ENABLE		 							0x300	
#define	MOTORCONTROLLER_STATUS_DRIVE_STOPPED					0x301	
#define	MOTORCONTROLLER_STATUS_OUTPUT_LIM_ACTIVEH			0x302	
#define	MOTORCONTROLLER_STATUS_OUTPUT_LIM_ACTIVEL 		0x303	
#define	MOTORCONTROLLER_STATUS_VACANT				 					0x304	
#define	MOTORCONTROLLER_STATUS_LIMIT_TO_CONT_CURRENT	0x305	
#define	MOTORCONTROLLER_STATUS_P_I										0x306	// Position control. End position � current(I) controller, directly coupled mode
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
#define	MOTORCONTROLLER_STATUS_SPEED_LIMH		 					0x312	
#define	MOTORCONTROLLER_STATUS_SPEED_LIML		 					0x313	
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
#define	MOTORCONTROLLER_STATUS_VACANT2	 							0x31E	
#define	MOTORCONTROLLER_STATUS_HANDWHL		 						0x31F	

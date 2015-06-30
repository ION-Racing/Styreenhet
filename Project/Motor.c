#include "stm32f4xx.h"
#include "Motor.h"
#include "CAN.h"
#include "ION_CAN.h"
#include "Pedals.h"

void BamocarTx(uint8_t motor, uint8_t reg, uint8_t length, uint8_t data[4]);

void BamocarTx(uint8_t motor, uint8_t reg, uint8_t length, uint8_t data[4]){
	
	length++; // REG Address
	
	uint8_t canData[6] = {reg};
	for(uint8_t i = 0; i<length; i++){
		canData[i+1] = data[i];
	}
	
	if(motor & MOTOR_LEFT){
		CANTx(CAN_MSG_MOTOR_LEFT_TX, length, canData);
	}
	if(motor & MOTOR_RIGHT){
		CANTx(CAN_MSG_MOTOR_RIGHT_TX, length, canData);
	}
}

void motorSetRPM(uint8_t motors, int16_t rpm){
	
	
}

void readRegisterRequest(uint8_t motor, uint8_t registerAddress, uint8_t interval){
	uint8_t readData[4] = {registerAddress, interval};
	BamocarTx(motor, 0x3D, 2, readData);
}

void writeRegister(uint8_t motor, uint8_t regAdr, uint8_t data1, uint8_t data2){
	uint8_t writeData[4] = {data1, data2};
	BamocarTx(motor, regAdr, 2, writeData);
}

void writeRegister16(uint8_t motor, uint8_t regAdr, uint16_t data){
	uint8_t writeData[4] = {data & 0xFF, data >> 8};
	BamocarTx(motor, regAdr, 2, writeData);
}

int16_t speed = 0;

void testMotor(){
	
	/*uint8_t drive_ready[2] = {0xE2, 0x00};
	BamocarTx(MOTOR_RIGHT, 0x3d, 2, drive_ready); // Read BTB
	
	Delay(0x7FFF);
	
	uint8_t enable_off[2] = {0x04, 0x00};
	BamocarTx(MOTOR_RIGHT, 0x51, 2, enable_off); // Set ENABLE OFF Mode bit
	
	Delay(0x7FFF);
	
	uint8_t read_RUN[2] = {0xE8, 0x00};
	BamocarTx(MOTOR_RIGHT, 0x3D, 2, read_RUN); // Read RUN state
	*/
	
	//uint8_t enable_on[4] = {0x00, 0x00};
	//BamocarTx(MOTOR_RIGHT, 0x51, 2, enable_on); // Enable motor (set all mode bits to 0)
	
	float pedal = getPedalValuef(PEDAL_TORQUE);
	uint16_t uns = (uint16_t)(pedal * 2000.0);
	speed = (int16_t)uns;
	
	writeRegister16(MOTOR_RIGHT, BAROCAM_REG_SPEED_CMD, speed);
	
	volatile uint32_t i = 0xFF;
	while(i-- > 0);
	
}
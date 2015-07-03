#include "stm32f4xx.h"
#include <stdbool.h>
#include "Motor.h"
#include "CAN.h"
#include "ION_CAN.h"
#include "Pedals.h"
#include "systick.h"
#include "startup.h"

void MotorSetRUN(FunctionalState state);
void MotorSetRPM(uint8_t motor, int16_t rpm);

void readRegisterRequest(uint8_t motor, uint8_t registerAddress, uint8_t interval);
uint8_t readRegisterBlocking(uint8_t motor, uint8_t registerAddress, uint32_t *data);

void writeRegister(uint8_t motor, uint8_t regAdr, uint8_t data1, uint8_t data2);
void writeRegister16(uint8_t motor, uint8_t regAdr, uint16_t data);
void writeRegister32(uint8_t motor, uint8_t regAdr, uint32_t data);

void BamocarTx(uint8_t motor, uint8_t reg, uint8_t length, uint8_t data[4]);

bool preArmReading = false;
uint32_t preArmRead = 0;

uint8_t MotorsPreArmCheck(void){

	// Read errors
	uint32_t errors[MOTORS_TOTAL] = {0, 0};
	//uint8_t readErrors = readRegisterBlocking(MOTOR_BOTH, BAMOCAR_REG_ERROR, errors);
	uint8_t readErrors = readRegisterBlocking(MOTOR_RIGHT, BAMOCAR_REG_ERROR, errors);
	if(readErrors != 0){
		ReportStartupError(STARTUP_ERR_MOTOR_READ);
		return 1;
	}
	
	// Check errors
	bool bitError = false;
	for(uint8_t i = 0; i<MOTORS_TOTAL; i++){
		for(uint8_t b = 0; b<16; b++){
			if(((errors[i] >> b) & 0x1) == 1){				
				ReportStartupError(STARTUP_ERR_MOTOR_READ + 1 + i * 16 + b);
				bitError = true;
			}
		}
	}
	if(bitError) return 1;
	
	
	return 0;
}

void MotorsEnable(void){
	// Set RUN-signal
	MotorSetRUN(ENABLE);
	
	// Wait 5ms
	delay_ms(5);
	
	// Set motorcontroller options
	writeRegister(MOTOR_BOTH, 0x51, 0x00, 0x00); // Enable motor
	readRegisterRequest(MOTOR_BOTH, 0x51, 0x00);
	
	// Set acceleration ramp
	writeRegister16(MOTOR_BOTH, BAMOCAR_REG_ACC_RAMP, 1000);
	writeRegister16(MOTOR_BOTH, BAMOCAR_REG_DEC_RAMP, 1000);
}

void MotorsDisable(void){
	// Disable RUN-signal
	MotorSetRUN(DISABLE);
	
	// Set zero torque
	MotorSetRPM(MOTOR_BOTH, 0);
	
	// Disable motors
	writeRegister16(MOTOR_BOTH, BAMOCAR_REG_MODE, BAMOCAR_MODE_ENABLE_OFF); 
}

void MotorLoop(void){
	
	int16_t torque = (int16_t)(getPedalValuef(PEDAL_TORQUE) * 10000.0f);
	
	MotorSetRPM(MOTOR_BOTH, torque);
}

void MotorSetRPM(uint8_t motor, int16_t rpm){
	
	/*float pedal = getPedalValuef(PEDAL_TORQUE);
	uint16_t uns = (uint16_t)(pedal * 2000.0);
	speed = (int16_t)uns;*/
	
	// We won't allow reverse for now
	if(rpm < 0) rpm = 0;
	
	writeRegister16(motor, BAMOCAR_REG_SPEED_CMD, rpm);
	
}

uint8_t blockingReads = 0;
uint8_t blockingReadAddress = 0;
uint32_t *blockingReadBuffer = 0;

void BamocarRx(uint8_t motorIdx, uint8_t reg, uint32_t data){
	
	if(blockingReads > 0 && blockingReadAddress == reg){
		*(blockingReadBuffer + motorIdx) = data;
		blockingReads--;		
	}
	
}

/* Private methods */

void MotorSetRUN(FunctionalState state){
	if(state == ENABLE){
		GPIOB->ODR |= GPIO_Pin_12;
	}
	else {
		GPIOB->ODR &= ~GPIO_Pin_12;		
	}
}

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

void readRegisterRequest(uint8_t motor, uint8_t registerAddress, uint8_t interval){
	uint8_t readData[4] = {registerAddress, interval};
	BamocarTx(motor, 0x3D, 2, readData);
}

uint8_t readRegisterBlocking(uint8_t motor, uint8_t registerAddress, uint32_t *data)
{
	blockingReads = (motor == MOTOR_BOTH ? 2 : 1);
	blockingReadBuffer = data;
	blockingReadAddress = registerAddress;
	
	readRegisterRequest(motor, registerAddress, 0x00);
	
	uint32_t timeout = 0x1FFFF; // ~8ms
	while(blockingReads > 0){
		if(timeout-- == 0) return 1;
	}
	
	return 0;	
}

void writeRegister(uint8_t motor, uint8_t regAdr, uint8_t data1, uint8_t data2){
	uint8_t writeData[4] = {data1, data2};
	BamocarTx(motor, regAdr, 2, writeData);
}

void writeRegister16(uint8_t motor, uint8_t regAdr, uint16_t data){
	uint8_t writeData[4] = {data & 0xFF, data >> 8};
	BamocarTx(motor, regAdr, 2, writeData);
}

void writeRegister32(uint8_t motor, uint8_t regAdr, uint32_t data){
	uint8_t writeData[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
	BamocarTx(motor, regAdr, 4, writeData);
}


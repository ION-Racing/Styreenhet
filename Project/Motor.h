
#define MOTORS_TOTAL	2

#define	MOTOR_LEFT	0x01
#define	MOTOR_RIGHT	0x02
#define	MOTOR_BOTH	0x03

#define	BAMOCAR_REG_ACC_RAMP		0x35
#define	BAMOCAR_REG_DEC_RAMP		0xED
#define	BAMOCAR_REG_SPEED_CMD		0x31
#define	BAMOCAR_REG_MODE			0x51
#define	BAMOCAR_REG_ERROR			0x8F

// BAMOCAR_REG_MODE
#define	BAMOCAR_MODE_ENABLE_OFF		0x04

// BAMOCAR_REG_ERROR
#define	BAMOCAR_ERR_BADPARAS		(1<<0)
#define	BAMOCAR_ERR_POWERFAULT		(1<<1)
#define	BAMOCAR_ERR_CANTIMEOUT		(1<<3)
#define	BAMOCAR_ERR_RESOSIGNAL		(1<<4)
#define	BAMOCAR_ERR_POWERVOLTAGE	(1<<5)
#define	BAMOCAR_ERR_MOTORTEMP		(1<<6)
#define	BAMOCAR_ERR_IDC				(1<<7)
#define	BAMOCAR_ERR_I_123			(1<<8)
#define	BAMOCAR_ERR_I_PEAK			(1<<9)
#define	BAMOCAR_ERR_RACEAWAY		(1<<10)
#define	BAMOCAR_ERR_CANINIT			(1<<11)
#define	BAMOCAR_ERR_SPIADCINIT		(1<<12)
#define	BAMOCAR_ERR_ROTOR			(1<<13)
#define	BAMOCAR_ERR_ADCTNT			(1<<14)
#define	BAMOCAR_ERR_BALLAST			(1<<15)



uint8_t MotorsPreArmCheck(void);

void MotorsEnable(void);
void MotorsDisable(void);

void MotorLoop(void);

void BamocarRx(uint8_t motor, uint8_t reg, uint32_t data);


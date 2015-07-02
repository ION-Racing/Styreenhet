
#define	MOTOR_LEFT	0x01
#define	MOTOR_RIGHT	0x02
#define	MOTOR_BOTH	0x03

#define	BAROCAM_REG_ACC_RAMP		0x35
#define	BAROCAM_REG_DEC_RAMP		0xED
#define	BAROCAM_REG_SPEED_CMD		0x31
#define	BAROCAM_REG_MODE			0x51

// BAROCAM_REG_MODE
#define	BAROCAM_MODE_ENABLE_OFF		0x04



void MotorsEnable(void);
void MotorsDisable(void);

void MotorLoop(void);



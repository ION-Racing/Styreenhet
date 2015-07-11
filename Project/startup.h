#define	STARTUP_ERR_TORQUE				1
#define	STARTUP_ERR_MOTOR_READ			2
// 3 - 18	Motor Left
// 19 - 34	Motor Right
#define	STARTUP_ERR_MOTOR_RDY_LEFT		35
#define	STARTUP_ERR_MOTOR_RDY_RIGHT		36
#define	STARTUP_ERR_MOTOR_ENABLE_LEFT	37
#define	STARTUP_ERR_MOTOR_ENABLE_RIGHT	38
#define	MOTOR_ERR_BSPD					39

#ifndef __STARTUP_H
#define __STARTUP_H

uint8_t RTDS(void);
uint8_t mcRun(void);

void checkStartup(void);
void ReportStartupError(uint8_t errorCode);

void Startup_START_Pushed(void);
void Startup_STOP_Pushed(void);

#endif

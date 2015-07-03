#define	STARTUP_ERR_TORQUE				1
#define	STARTUP_ERR_MOTOR_READ			2
// 3 - 34	Motor Left
// 35 - 66	Motor Right

#ifndef __STARTUP_H
#define __STARTUP_H

uint8_t RTDS(void);
uint8_t mcRun(void);

void checkStartup(void);
void ReportStartupError(uint8_t errorCode);

void Startup_START_Pushed(void);
void Startup_STOP_Pushed(void);

#endif

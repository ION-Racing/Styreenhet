#ifndef __STARTUP_H
#define __STARTUP_H

uint8_t RTDS(void);
uint8_t mcRun(void);
void checkStartup(void);

void Startup_START_Pushed(void);
void Startup_STOP_Pushed(void);

#endif

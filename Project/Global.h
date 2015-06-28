/* Structure for wheelsensor 1 and wheelsensor 2.
The values get set by the GPIO interrupt in EXTI.C
and is used in TxWheelsensor.c.
*/
#include "stdint.h"

typedef struct{
	uint8_t 	state1		;	// state of wheelsensor 1
	uint32_t 	period1		;	// Time between triggers for wheelsensor 1.
	uint8_t		state2		;	// state of wheelsensor 2.
	uint32_t 	period2		;	// Time between triggers for wheelsensor 2.
} wheeld;

extern wheeld wheel;

// Enum for the cars current state
typedef enum CarState {
	DISARMED = 0,
	ARMING,
	ARMED,
	CRITICAL_ERROR
} CarState;

extern CarState carState;

// Pedal values
#define	PEDAL_TORQUE	0
#define	PEDAL_BRAKE		1

extern uint16_t pedalValues[2];

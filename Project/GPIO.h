void InitGPIO(void);

void SetRUN(FunctionalState state);
void SetRTDS(FunctionalState state);

#define	LED_GREEN	1
#define	LED_RED		2
#define	LED_BLUE	3

void LED_SetState(uint8_t led, FunctionalState state);

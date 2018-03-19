#ifndef BUZZER_H_
#define BUZZER_H_

#include "stdint.h"
#include "main.h"
#define BUZZER_ON()		(BUZZER_GPIO_Port->BSRR = BUZZER_Pin)
#define BUZZER_OFF()	(BUZZER_GPIO_Port->BSRR = (uint32_t)BUZZER_Pin << 16)

typedef enum {
	BEEP_OFF = 0,
	BEEP_SYSTEM_INIT_START = 0b10001010,
	BEEP_SYSTEM_INIT_FINISH = 0b10101001,
	BEEP1_I  = 0b10000000,
	BEEP2_I  = 0b10100000,
	BEEP3_I  = 0b10101000,
	BEEP4_I  = 0b10101010,

	BEEP1_S  = 0b10000000,
	BEEP2_S  = 0b11000000,
	BEEP3_S  = 0b11110000,
	BEEP4_S  = 0b11111100,
	BEEP_ON  = 0b11111111
} tBEEP_TYPE;

extern uint8_t BeepOCF(void);
extern void Beep(tBEEP_TYPE cmd);// repetition
extern void BeepOS(tBEEP_TYPE cmd);// one time
extern void BeepEx(void);
#endif /* BUZZER_H_ */

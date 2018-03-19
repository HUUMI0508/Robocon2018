#include "Buzzer.h"

static tBEEP_TYPE type = BEEP_OFF;
static uint8_t temp = 0b10000000;
static uint8_t one_shot_flg = 0;

uint8_t BeepOCF(void) {
	return temp;
}

void Beep(tBEEP_TYPE cmd) {
	one_shot_flg = 0;
	if (type != cmd) {
		type = cmd;
		temp = 0b10000000;
	}
}
void BeepOS(tBEEP_TYPE cmd) {
	one_shot_flg = 1;
	if (type != cmd || temp == 0) {
		type = cmd;
		temp = 0b10000000;
	}
}
void BeepEx(void){
	if (type & temp) {
		BUZZER_ON();
	} else {
		BUZZER_OFF();
	}
	if (temp != 0b00000001) {
		temp >>= 1;
	} else {
		if (one_shot_flg) {
			temp = 0;
		} else {
			temp = 0b10000000;
		}
	}
}

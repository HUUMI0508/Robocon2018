#include "DualShock.h"
#include "xuart.h"
#include "Buzzer.h"

tDSParm hDS;
static uint8_t buf[8], flg_connection_ok = 0, check = 0;
static UART_HandleTypeDef *huartDS;
static float DimensionlessAndBacklash(int8_t dat);
static uint8_t getsum(void *buf, size_t size);

void ReStartDS(void) {
	check = 0;
	HAL_UART_Receive_IT(huartDS, buf, 1);
}

uint8_t InitDS(UART_HandleTypeDef *huart) {
	static uint8_t time_out = 0;

	hDS.BUTTON.ButtonData = 0;
	hDS.ANALOG.LX = 0;
	hDS.ANALOG.LY = 0;
	hDS.ANALOG.RX = 0;
	hDS.ANALOG.RY = 0;
	huartDS = huart;

	printf("DualShock connecting check...\r\n");
	ReStartDS();
	while (flg_connection_ok == 0) {
		if (BeepOCF() == 0) {//0.4s
			BeepOS(BEEP2_I);
			if (time_out++ > 25) {//10s
				printf("DualShock connect error...\r\n");
				return 1;
			}
		}
	}
	printf("DualShock connect successful...\r\n");
	return 0;
}

void getDSdata(void) {
	static uint8_t dat[8] = {0};

	HAL_UART_Receive_IT(huartDS, buf, 1);
	dat[check] = buf[0];
	switch (check) {
	case 0:
		if (dat[0] == 0x80) {
			check++;
		}
		break;
	case 7:
		if (dat[7] == (getsum(dat, 7) & 0x7f)) {
			hDS.BUTTON.ButtonHighData = dat[1];
			hDS.BUTTON.ButtonLowData = dat[2];
			hDS.ANALOG.LX = DimensionlessAndBacklash(dat[3]);
			hDS.ANALOG.LY = DimensionlessAndBacklash(dat[4]);
			hDS.ANALOG.RX = DimensionlessAndBacklash(dat[5]);
			hDS.ANALOG.RY = DimensionlessAndBacklash(dat[6]);
			flg_connection_ok = 1;
		}
		dat[0] = 0;
		check = 0;
		break;
	default:
		check++;
		break;
	}
}

static float DimensionlessAndBacklash(int8_t dat) {
	float val;

	if ((dat < BACKLASH) && (dat > -BACKLASH)) {
		dat = 0;
	} else {
		dat += (dat > 0) ? (-BACKLASH) : (BACKLASH);
	}
	val = (float) dat / (float) (128 - BACKLASH);
	return val;
}

static uint8_t getsum(void *buf, size_t size) {
	const uint8_t *p = (uint8_t*) buf;
	uint8_t ret = 0;

	for (; size; size--) {
		ret += *p++;
	}
	return ret;
}

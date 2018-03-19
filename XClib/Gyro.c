#include "../Inc/usart.h"
#include "Gyro.h"

tGyroParm hgyro;
static UART_HandleTypeDef *huartGyro;
static uint8_t data;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void InitGyro(UART_HandleTypeDef *huart) {
	HAL_Delay(1000);
	huartGyro = huart;
	hgyro.qDeg = 0;
	hgyro.qRad = 0;
	hgyro.qFreq = 0;
	hgyro.qaccX  = 0;
	hgyro.qaccY  = 0;
	hgyro.qaccZ  = 0;
	HAL_UART_Receive_IT(huartGyro, &data, 1);
}

void CalcGyro(void){
	const int32_t hogehoge = -(100 * 100) * (100 * 100);
	static uint16_t buf[13] = {0};
	static uint8_t cnt = 0, check = 0;
	static int8_t rotation_cnt = 0;
	static int16_t temp = 0, past_temp = 0;

	HAL_UART_Receive_IT(huartGyro, &data, 1);
	switch (check) {
	case 0:
		if (data == 0xaa) {
			check++;
		}
		break;
	case 1:
		if (data == 0x00) {
			check++;
		} else {
			check = 0;
			cnt = 0;
		}
		break;
	case 2:
		buf[cnt++] = data;
		if (cnt >= 13) {
			for (uint8_t data = 0, i = 0;i < 11; i++) {
				data += buf[i];
			}
			if ((data & 0xff) == buf[12]) {
				temp = (int16_t)((buf[2] << 8) | buf[1]);
				if ((int32_t)temp * (int32_t)past_temp < hogehoge) {
					past_temp = temp;
					if (temp < 0) {
						rotation_cnt--;
						temp *= -1;
					} else {
						rotation_cnt++;
					}
				} else {
					past_temp = temp;
				}
				hgyro.qDeg = 360.0 * rotation_cnt - ((float)temp / 100);
				hgyro.qRad = hgyro.qDeg / 180.0 * M_PI;
				temp = (int16_t)((buf[4] << 8) | buf[3]);
				hgyro.qFreq = (float)temp / -36000;
				hgyro.qaccX = (buf[6] << 8) | buf[5];
				hgyro.qaccY  = (buf[8] << 8) | buf[7];
				hgyro.qaccZ  = (buf[10] << 8) | buf[9];
			}
			check = 0;
			cnt = 0;
		}
		break;
	}
}

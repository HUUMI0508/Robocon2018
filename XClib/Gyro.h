#ifndef _GYRO_H_
#define _GYRO_H_
// R1350n
// sampling freq. 100Hz
typedef struct {
	float qDeg;//deg
	float qRad;//rad
	float qFreq;//rps
	int16_t qaccX;//mg
	int16_t qaccY;//mg
	int16_t qaccZ;//mg
} tGyroParm;

extern tGyroParm hgyro;

extern void InitGyro(UART_HandleTypeDef *huart);
extern void CalcGyro(void);
#endif

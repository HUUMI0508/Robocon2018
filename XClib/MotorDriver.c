#include "../Inc/can.h"
#include "xcan.h"
#include "MotorDriver.h"

void InitMotorDriver(CAN_HandleTypeDef* hcan, tMD *pParm, tMTR_ID id, tMTR_CHANNEL ch, tMTR_MODE mode) {
	const uint8_t dlc[16] = { 1, 3, 5, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	pParm->TxMessage.StdId = id;
	pParm->TxMessage.RTR = CAN_RTR_DATA;
	pParm->TxMessage.IDE = CAN_ID_STD;
	pParm->TxMessage.DLC = dlc[mode];
	pParm->TxMessage.Data[0] = ch | mode;
	for (uint8_t i = 1; i < 8; i++) {
		pParm->TxMessage.Data[i] = 0;
	}
	pParm->qCANcycleNum = AddToTheCANcycle(hcan, &pParm->TxMessage, 0);
}

void MotorDriveOpenLoop(tMD *pParm, int16_t pwm) {
	if ((pParm->TxMessage.Data[0] & MODE_MASK) != MODE_OpenLoop) {
		return;
	}
	if (pwm > MAX_DUTY) {
		pwm = MAX_DUTY;
	} else if (pwm < -MAX_DUTY) {
		pwm = -MAX_DUTY;
	}
	pParm->TxMessage.Data[1] = (uint8_t) ((pwm >> 8) & 0xff);
	pParm->TxMessage.Data[2] = (uint8_t) (pwm & 0xff);

}

void MotorDriveClosedLoop(tMD *pParm, float rps) {
	tICF c;
	c.fval = rps;
	if ((pParm->TxMessage.Data[0] & MODE_MASK) != MODE_ClosedLoop) {
		return;
	}
	if (c.fval > MAX_RPS) {
		c.fval = MAX_RPS;
	} else if (c.fval < -MAX_RPS) {
		c.fval = -MAX_RPS;
	}
	pParm->TxMessage.Data[1] = (uint8_t) ((c.ival >> 24) & 0xff);
	pParm->TxMessage.Data[2] = (uint8_t) ((c.ival >> 16) & 0xff);
	pParm->TxMessage.Data[3] = (uint8_t) ((c.ival >> 8) & 0xff);
	pParm->TxMessage.Data[4] = (uint8_t) (c.ival & 0xff);
}

void MotorDriveServo(tMD *pParm, tICF angle, float max_rps) {
	uint16_t temp;

	pParm->TxMessage.Data[1] = (uint8_t) ((angle.ival >> 24) & 0xff);
	pParm->TxMessage.Data[2] = (uint8_t) ((angle.ival >> 16) & 0xff);
	pParm->TxMessage.Data[3] = (uint8_t) ((angle.ival >> 8) & 0xff);
	pParm->TxMessage.Data[4] = (uint8_t) (angle.ival & 0xff);
	temp = (uint16_t) (max_rps * 100);
	pParm->TxMessage.Data[5] = (uint8_t) ((temp >> 8) & 0xff);
	pParm->TxMessage.Data[6] = (uint8_t) (temp & 0xff);
}

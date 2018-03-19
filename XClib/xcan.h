#ifndef XCAN_H_
#define XCAN_H_
#include "../Inc/can.h"

#define MessageBoxSUM 16

typedef struct {
	CanTxMsgTypeDef *pTxMessage[MessageBoxSUM];
//	CanRxMsgTypeDef *pRxMessage[MessageBoxSUM];
	uint8_t qNodeSum;
	uint8_t qSendingNum;
	uint8_t qOneTimeOnly[MessageBoxSUM];
} tXCAN;

#if defined(__STM32F1xx_HAL_H)
extern tXCAN hxcan1;
#elif defined(__STM32F7xx_HAL_H)
extern tXCAN hxcan1;
extern tXCAN hxcan2;
extern tXCAN hxcan3;
#endif

/**
 * @brief Init
 * @param *hParm
 */
extern void InitCAN(CAN_HandleTypeDef* hcan);

extern uint8_t AddToTheCANcycle(CAN_HandleTypeDef* hcan, CanTxMsgTypeDef *pTxMessage, uint8_t one_time_only);

extern void RemoveFromCANcycle(tXCAN *pParm, uint8_t num);

extern void TransmitCAN(CAN_HandleTypeDef* hcan);

extern void StartCAN(CAN_HandleTypeDef* hcan);

extern void ReConnectCAN(CAN_HandleTypeDef* hcan);
#endif /* XCAN_H_ */

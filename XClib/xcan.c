#include "../Inc/can.h"
#include "xcan.h"

#if defined(__STM32F1xx_HAL_H)
tXCAN hxcan1;
#elif defined(__STM32F7xx_HAL_H)
tXCAN hxcan1;
tXCAN hxcan2;
tXCAN hxcan3;
#endif

void TransmitCAN(CAN_HandleTypeDef *hcan) {
	tXCAN *pParm;

#if defined(__STM32F1xx_HAL_H)
	if (hcan->Instance == CAN1) {
		pxcanParm = &hxcan1;
	} else {
		return;
	}
#elif defined(__STM32F7xx_HAL_H)
	if (hcan->Instance == CAN1) {
		pParm = &hxcan1;
	} else if (hcan->Instance == CAN2) {
		pParm = &hxcan2;
	} else if (hcan->Instance == CAN3) {
		pParm = &hxcan3;
	} else {
		return;
	}
#endif

	// data set
	hcan->pTxMsg = pParm->pTxMessage[pParm->qSendingNum];
	if (pParm->qOneTimeOnly[pParm->qSendingNum]) {
		RemoveFromCANcycle(pParm, pParm->qSendingNum);
	}
	//send data
	HAL_CAN_Transmit_IT(hcan);
	// id count up
	if (++pParm->qSendingNum > pParm->qNodeSum - 1) {
		pParm->qSendingNum = 0;
	}

}

void RemoveFromCANcycle(tXCAN *pParm, uint8_t num) {
	pParm->pTxMessage[num] = NULL;
	pParm->qOneTimeOnly[num] = 0;
	for (uint8_t i = num; i < pParm->qNodeSum - 1; i++) {
		pParm->pTxMessage[i] = pParm->pTxMessage[i + 1];
		pParm->qOneTimeOnly[i] = pParm->qOneTimeOnly[i + 1];
	}
	pParm->qNodeSum--;
}

uint8_t AddToTheCANcycle(CAN_HandleTypeDef* hcan, CanTxMsgTypeDef *pTxMessage, uint8_t one_time_only) {
#if defined(__STM32F1xx_HAL_H)
	if (hcan->Instance == CAN1) {
		hxcan1.pTxMessage[hxcan1.qNodeSum++] = pTxMessage;
		return (hxcan1.qNodeSum - 1);
	}
#elif defined(__STM32F7xx_HAL_H)
	if (hcan->Instance == CAN1) {
		hxcan1.qOneTimeOnly[hxcan1.qNodeSum] = one_time_only;
		hxcan1.pTxMessage[hxcan1.qNodeSum] = pTxMessage;
		hxcan1.qNodeSum++;
		return (hxcan1.qNodeSum - 1);
	} else if (hcan->Instance == CAN2) {
		hxcan2.qOneTimeOnly[hxcan2.qNodeSum] = one_time_only;
		hxcan2.pTxMessage[hxcan2.qNodeSum] = pTxMessage;
		hxcan2.qNodeSum++;
		return (hxcan2.qNodeSum - 1);
	} else if (hcan->Instance == CAN3) {
		hxcan3.qOneTimeOnly[hxcan3.qNodeSum] = one_time_only;
		hxcan3.pTxMessage[hxcan3.qNodeSum] = pTxMessage;
		hxcan3.qNodeSum++;
		return (hxcan3.qNodeSum - 1);
	}
#endif
	return 0;
}

static CanTxMsgTypeDef TxMessage;
static CanRxMsgTypeDef RxMessage;
void InitCAN(CAN_HandleTypeDef* hcan) {
	static uint8_t ini = 0;
	CAN_FilterConfTypeDef sFilterConfig;
	if (ini == 0) {
		ini = 1;
#if defined(__STM32F1xx_HAL_H)
		hxcan1.qNodeSum = 0;
		hxcan1.qSendingNum = 0;
		for (uint8_t i = 0; i < MessageBoxSUM; i++) {
			hxcan1.pTxMessage[i] = NULL;
			hxcan1.qSendEnable[i] = 0;
		}
#elif defined(__STM32F7xx_HAL_H)
		hxcan1.qNodeSum = 0;
		hxcan2.qNodeSum = 0;
		hxcan3.qNodeSum = 0;
		hxcan1.qSendingNum = 0;
		hxcan2.qSendingNum = 0;
		hxcan3.qSendingNum = 0;
		for (uint8_t i = 0; i < MessageBoxSUM; i++) {
			hxcan1.pTxMessage[i] = NULL;
			hxcan2.pTxMessage[i] = NULL;
			hxcan3.pTxMessage[i] = NULL;
			hxcan1.qOneTimeOnly[i] = 0;
			hxcan2.qOneTimeOnly[i] = 0;
			hxcan3.qOneTimeOnly[i] = 0;
		}
#endif
	}
#if defined(__STM32F1xx_HAL_H)
	if (hcan->Instance == CAN1) {
		hcan->pTxMsg = &TxMessage;
		hcan->pRxMsg = &RxMessage;
	}
#elif defined(__STM32F7xx_HAL_H)
	if (hcan->Instance == CAN1) {
		hcan->pTxMsg = &TxMessage;
		hcan->pRxMsg = &RxMessage;
	} else if (hcan->Instance == CAN2) {
		hcan->pTxMsg = &TxMessage;
		hcan->pRxMsg = &RxMessage;
	} else if (hcan->Instance == CAN3) {
		hcan->pTxMsg = &TxMessage;
		hcan->pRxMsg = &RxMessage;
	}
#endif
	/*** Configure the CAN Filter ***/
	sFilterConfig.FilterNumber = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 14;
	if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
		Error_Handler();
}

void StartCAN(CAN_HandleTypeDef* hcan) {
	if (hcan->Instance == CAN1) {
		hcan->pTxMsg = hxcan1.pTxMessage[0];
	} else if (hcan->Instance == CAN2) {
		hcan->pTxMsg = hxcan2.pTxMessage[0];
	} else if (hcan->Instance == CAN3) {
		hcan->pTxMsg = hxcan3.pTxMessage[0];
	}
	HAL_CAN_Transmit_IT(hcan);	// can start
}

void ReConnectCAN(CAN_HandleTypeDef* hcan) {
	if (hcan1.ErrorCode == HAL_CAN_ERROR_NONE) {
		return;
	}
	StartCAN(hcan);
}

#if 0
HAL_CAN_Transmit_IT(hcan);
if(HAL_CAN_Transmit(&hcan, 10) != HAL_OK)
{
	/* Transmition Error */
	LED_RED(1);
} else {
	LED_RED(0);
}
if(HAL_CAN_GetState(&hcan) != HAL_CAN_STATE_READY)
{
	LED_ORANGE(1);
} else {
	LED_ORANGE(0);
}
LED_GREEN_TOGGLE();
HAL_Delay(300);

if (HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK) {Error_Handler();}
#if 0
if(HAL_CAN_Receive(&hcan, CAN_FIFO0,10) != HAL_OK)
{
	/* Reception Error */
	temp = 1;
}

if(HAL_CAN_GetState(&hcan) != HAL_CAN_STATE_READY)
{
	temp = 1;
}

if(hcan.pRxMsg->StdId != 0x11)
{
	temp = 1;
}

if(hcan.pRxMsg->IDE != CAN_ID_STD)
{
	temp = 1;
}

if(hcan.pRxMsg->DLC != 2)
{
	temp = 1;
}

if((hcan.pRxMsg->Data[0]<<8|RxMessage.Data[1]) != 0xCAFE)
{
	temp = 1;
}

if (temp) {
	temp = 0;
	LED_RED(1);
	LED_GREEN(0);
} else {
	LED_RED(0);
	LED_GREEN(1);
}
#endif
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle)
{
	if ((CanHandle->pRxMsg->StdId == 0x11) && (CanHandle->pRxMsg->IDE == CAN_ID_STD) && (CanHandle->pRxMsg->DLC == 2))
	{
		LED_GREEN(1);
		temp = CanHandle->pRxMsg->Data[0];
	} else {
		LED_GREEN(0);
	}

	/* Receive */
	if (HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0) != HAL_OK)
	{
		/* Reception Error */
		LED_RED(1);
	} else {
		LED_RED(0);
	}
}
#endif

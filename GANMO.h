#ifndef _GANMO_H_
#define _GANMO_H_
#include "../Inc/main.h"
#define ARM_MATH_CM7
#include "arm_math.h"
#include "arm_const_structs.h"
#include "Buzzer.h"
#include "DualShock.h"
#include "Encoder.h"
#include "Gyro.h"
#include "MotorControl.h"
#include "MotorDriver.h"
#include "Odometry.h"
#include "Omni.h"
#include "PIcontrol.h"
#include "SolenoidValveDriver.h"
#include "xcan.h"
#include "xuart.h"
#include "LRF.h"
#include "User.h"

#define setPinStateHigh(__GPIO__, __GPIO_Pin__) (((__GPIO__)->BSRR) = (__GPIO_Pin__))
#define setPinStateLow(__GPIO__, __GPIO_Pin__)  (((__GPIO__)->BSRR) = (uint32_t)(__GPIO_Pin__) << 16U)
#define togglePinState(__GPIO__, __GPIO_Pin__)  (((__GPIO__)->ODR) ^= (__GPIO_Pin__))
#define getPinState(__GPIO__, __GPIO_Pin__)     (((__GPIO__)->IDR) & (__GPIO_Pin__))

// LED GREEN
#define LED_GREEN_HIGH()	setPinStateHigh(LED_GREEN_GPIO_Port, LED_GREEN_Pin)
#define LED_GREEN_LOW()		setPinStateLow(LED_GREEN_GPIO_Port, LED_GREEN_Pin)
#define LED_GREEN_T()		togglePinState(LED_GREEN_GPIO_Port, LED_GREEN_Pin)
#define	LED_GREEN(X)		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, X)
// LED RED
#define LED_RED_HIGH()		setPinStateHigh(LED_RED_GPIO_Port, LED_RED_Pin)
#define LED_RED_LOW()		setPinStateLow(LED_RED_GPIO_Port, LED_RED_Pin)
#define LED_RED_T()			togglePinState(LED_RED_GPIO_Port, LED_RED_Pin)
#define	LED_RED(X)			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, X)
// LED ORANGE
#define LED_ORANGE_HIGH()	setPinStateHigh(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin)
#define LED_ORANGE_LOW()	setPinStateLow(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin)
#define LED_ORANGE_T()		togglePinState(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin)
#define	LED_ORANGE(X)		HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, X)
// GYRO
#define GYRO_nRST_HIGH()	setPinStateHigh(GYRO_nRST_GPIO_Port, GYRO_nRST_Pin)
#define GYRO_nRST_LOW()		setPinStateLow(GYRO_nRST_GPIO_Port, GYRO_nRST_Pin)
#define GYRO_nRST_T()		togglePinState(GYRO_nRST_GPIO_Port, GYRO_nRST_Pin)
#define	GYRO_nRST(X)		HAL_GPIO_WritePin(GYRO_nRST_GPIO_Port, GYRO_nRST_Pin, X)
// RS485
#define RE_DE_HIGH()		setPinStateHigh(RE_DE_GPIO_Port, RE_DE_Pin)
#define RE_DE_LOW()			setPinStateLow(RE_DE_GPIO_Port, RE_DE_Pin)
#define RE_DE_T()			togglePinState(RE_DE_GPIO_Port, RE_DE_Pin)
#define	RE_DE(X)			HAL_GPIO_WritePin(RE_DE_GPIO_Port, RE_DE_Pin, X)
// PS
#define MCPS_HIGH()			setPinStateHigh(MCPS_GPIO_Port, MCPS_Pin)
#define MCPS_LOW()			setPinStateLow(MCPS_GPIO_Port, MCPS_Pin)
#define MCPS_T()			togglePinState(MCPS_GPIO_Port, MCPS_Pin)
#define MCPS(X)				HAL_GPIO_WritePin(MCPS_GPIO_Port, MCPS_Pin, X)
// KINTE
#define KINTE()				getPinState(KINTE_GPIO_Port, KINTE_Pin)
// SW1
#define SW1()				getPinState(SW1_GPIO_Port, SW1_Pin)
// SW2
#define SW2()				getPinState(SW2_GPIO_Port, SW2_Pin)
// LIMIT AH
#define LIMIT_AH()			getPinState(LIMIT_AH_GPIO_Port, LIMIT_AH_Pin)
// LIMIT AL
#define LIMIT_AL()			getPinState(LIMIT_AL_GPIO_Port, LIMIT_AL_Pin)
// LIMIT BH
#define LIMIT_BH()			getPinState(LIMIT_BH_GPIO_Port, LIMIT_BH_Pin)
// LIMIT BL
#define LIMIT_BL()			getPinState(LIMIT_BL_GPIO_Port, LIMIT_BL_Pin)


#define INPUT_A3()			getPinState(A3_GPIO_Port, A3_Pin)


enum {
	FALSE = 0,
	TRUE
};

enum {
	LOW = 0,
	HIGH
};

#if 0
void PinModeChange(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t mode) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_Pin;
	if (mode == GPIO_MODE_INPUT) {
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;

	} else if (mode == GPIO_MODE_OUTPUT_PP) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	}
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
#endif

uint8_t rx_buf[256];

#ifndef WHEEL_ONLY
//#include "User_mura.h"
#endif

extern void InitGANMO(void);
#endif

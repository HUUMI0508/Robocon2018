#include "../Inc/tim.h"
#include "Encoder.h"

void InitENC(tEncParm *pParm, tENCSelect EncTimCh, float ts_ms, float Gm, float Ge, float ppr, tDIRECTION DoR) {
	float CutFreq;// [rad/s]

	pParm->qTs = ts_ms / 1000;
	CutFreq = 2.0 * M_PI / (pParm->qTs * 10.0);// 2 * pi * f[rad/s]
	//	y = (2.0 * gpd * (u - uZ1) + (2.0 - gpd * Ts)*yZ1) / (2.0 + gpd * Ts);	//‘oˆêŽŸ•ÏŠ·‚ÆLPF‚ð—p‚¢‚½”÷•ª
	pParm->qCalcConst[0] = 2.0 * CutFreq / (pParm->qTs * CutFreq + 2.0);
	pParm->qCalcConst[1] = (2.0 - CutFreq * pParm->qTs) / (pParm->qTs * CutFreq + 2.0);
	pParm->qCnvParm = Gm / (4.0 * ppr * Ge) * DoR;
	pParm->qGm = Gm;
	pParm->qGe = Ge;
	pParm->qRev = 0;
	pParm->qDeg = 0;
	pParm->qRad = 0;
	pParm->qFreq = 0;
	pParm->qOmega = 0;
	pParm->qPastRev = 0;
	pParm->qENCCount = 0;
	pParm->qENCChannel = EncTimCh;
#if defined(__STM32F1xx_HAL_H)
	switch (EncTimCh) {
	case ENC1:
		MX_TIM3_Init();
		pParm->Instance = htim3.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		break;
	case ENC2:
		MX_TIM2_Init();
		pParm->Instance = htim2.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
		break;
	}
#elif defined(__STM32F7xx_HAL_H)
	switch (EncTimCh) {
		case ENC1: //TIM1
		MX_TIM1_Init();
		pParm->Instance = htim1.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
		break;
		case ENC2://TIM8
		MX_TIM8_Init();
		pParm->Instance = htim8.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
		break;
		case ENC3://TIM4
		MX_TIM4_Init();
		pParm->Instance = htim4.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
		break;
		case ENC4://TIM3
		MX_TIM3_Init();
		pParm->Instance = htim3.Instance;
		pParm->Instance->CNT = 0x8000;
		HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		break;
	}
#endif
}

void CalcENC(tEncParm *pParm) {
	pParm->qENCCount += pParm->Instance->CNT - 0x8000;
	pParm->Instance->CNT = 0x8000; // timer counter reset
	pParm->qRev = pParm->qCnvParm * (float)pParm->qENCCount; // [rotation]
	pParm->qDeg = pParm->qRev * 360.0; // [deg]
	pParm->qRad = pParm->qRev * (2.0 * M_PI); // [rad]
	pParm->qFreq = pParm->qCalcConst[0] * (pParm->qRev - pParm->qPastRev) + pParm->qCalcConst[1] * pParm->qFreq;
	pParm->qOmega = pParm->qFreq * (2.0 * M_PI);
	pParm->qPastRev = pParm->qRev;
}

void ClearENC(tEncParm *pParm) {
	pParm->qRev = 0;
	pParm->qDeg = 0;
	pParm->qRad = 0;
	pParm->qFreq = 0;
	pParm->qOmega = 0;
	pParm->qPastRev = 0;
	pParm->Instance->CNT = 0x8000;
	pParm->qENCCount = 0;
}

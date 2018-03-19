#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "../Inc/tim.h"
#include "stdint.h"

#if defined(__STM32F1xx_HAL_H)
typedef enum {
	ENC1 = 0,
	ENC2
} tENCSelect;
typedef enum {
	CLOCKWISE = 1,
	COUNTERCLOCKWISE = -1
} tDIRECTION;
#elif defined(__STM32F7xx_HAL_H)
typedef enum {
	ENC1 = 0,
	ENC2,
	ENC3,
	ENC4
}tENCSelect;
typedef enum {
	CLOCKWISE = -1,
	COUNTERCLOCKWISE = 1
} tDIRECTION;
#endif

//#ifndef M_PI
#define M_PI 3.14159265358979323846
//#endif

typedef struct {
	float       qTs;				// Control cycle[s]
	float       qGm;				// motor to out Gear Ratio
	float       qGe;				// motor to enc gear
	float       qCnvParm;			// qGearRatio * 4.0 * ENCppr
	float       qCalcConst[2];		//
	float       qRev;				// [revolution]
	float       qDeg;				// [deg]
	float       qRad;				// [rad]
	float       qFreq;				// [rps]
	float       qOmega;			// [rad/sec]
	int32_t      qENCCount;			//
	float       qPastRev;			// [revolution]
	tENCSelect   qENCChannel;		// Encoder Channel
	TIM_TypeDef *Instance;			//
	float       qDebug;			// Variable for debugging
} tEncParm;

#if defined(__STM32F7xx_HAL_H)
tEncParm henc1;
tEncParm henc2;
tEncParm henc3;
tEncParm henc4;
#endif

/**
 * @brief Function to initialize the encoder
 * @param *pParm		see definition above
 * @param ENCTimChannel	see definition above
 * @param ts_ms			control cycle[ms]
 * @param gear_ratio	gear ratio
 * @param ppr			encoder pulse value [rev/pulse]
 */
extern void InitENC(tEncParm *pParm, tENCSelect EncTimCh, float ts_ms, float Gm, float Ge, float ppr, tDIRECTION DoR);

/**
 * @brief Function to calculate encoder state
 * @note you must call this function in you set control cycle ts[ms]
 * @param *pParm		see definition above
 */
extern void CalcENC(tEncParm *pParm);

/**
 * @brief Clear
 * @param *pParm		see definition above
 */
extern void ClearENC(tEncParm *pParm);
#endif /* _ENCODER_H_ */

#ifndef _PIcontrol_H_
#define _PIcontrol_H_

#include "stdint.h"

typedef struct {
	float qTs;			// Control cycle
	float qPastError;	// Past error
	float qKp;			// Proportional Gain
	float qKi;			// Integral Gain
	uint16_t qOutMax;		// PI Output maximum limit
	int16_t qOutMin;		// PI Output minimum limit
	float qInRef;		// Reference
	float qInMeas;		// Measurement
	float qOut;			// PI Output
	float qCalcConst[2];
	float qDebug;		// Variable for debugging
} tPIParm;

#define setInRef(__HANDLE__, __INREF__)   (((__HANDLE__)->qInRef)  = (__INREF__))
#define setInMeas(__HANDLE__, __INMEAS__) (((__HANDLE__)->qInMeas) = (__INMEAS__))

/**
 * @brief function to init the PI Controller
 * @param *pParm	see definition above
 * @param Kp		Proportional Gain
 * @param Ki		Integral Gain
 * @param max		PI Output maximum limit
 * @param min		PI Output minimum limit
 * @param ts		Control cycle[ms] 1ms
 */
extern void InitPI(tPIParm *pParm, float Kp, float Ki, int16_t max, int16_t min, float ts_ms);
extern void CalcPIconst(tPIParm *pParm);
extern void ResetPIParm(tPIParm *pParm);
/**
 * @brief function to calculate the output of the PI
 * @note you must call this func. in you set up control cycle.
 * @param *pParm	see definition above
 */
extern void CalcPI(tPIParm *pParm, float u);

#endif /* _PIcontrol_H_ */

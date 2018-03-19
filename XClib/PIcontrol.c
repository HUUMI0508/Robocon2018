#include "PIcontrol.h"

void InitPI(tPIParm *pParm, float Kp, float Ki, int16_t max, int16_t min, float ts_ms) {
	pParm->qKp = Kp;
	pParm->qKi = Ki;
	pParm->qOutMax = max;
	pParm->qOutMin = min;
	pParm->qTs = ts_ms / 1000;
	pParm->qInRef = 0;
	pParm->qInMeas = 0;
	pParm->qPastError = 0;
	pParm->qOut = 0;
	CalcPIconst(pParm);
}

void CalcPIconst(tPIParm *pParm) {
	pParm->qCalcConst[0] = (pParm->qKi * pParm->qTs + 2.0 * pParm->qKp) / 2.0;
	pParm->qCalcConst[1] = (pParm->qKi * pParm->qTs - 2.0 * pParm->qKp) / 2.0;
}

void ResetPIParm(tPIParm *pParm) {
	pParm->qPastError = 0;
	pParm->qOut = 0;
}

/* CalcPI - function to calculate the output of the PI */
void CalcPI(tPIParm *pParm, float u) {
	//y  = ((Ki*T+2*Kp)*x+(Ki*T-2*Kp)*xZ1)/2 + yZ1;
//	pParm->qOut += ((pParm->qKi * pParm->qTs + 2.0 * pParm->qKp) * x + (pParm->qKi * pParm->qTs - 2.0 * pParm->qKp) * pParm->qPastError) / 2.0;
	pParm->qOut += (pParm->qCalcConst[0] * u + pParm->qCalcConst[1] * pParm->qPastError);
	pParm->qPastError = u;
	//limit the output between the allowed limits
	//pParm->qOut is the PI output
	if (pParm->qOut > pParm->qOutMax) {
		pParm->qOut = pParm->qOutMax;
	} else if (pParm->qOut < pParm->qOutMin) {
		pParm->qOut = pParm->qOutMin;
	}
}

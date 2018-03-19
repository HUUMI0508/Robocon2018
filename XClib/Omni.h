#ifndef OMNI_H_
#define OMNI_H_
#include "stdint.h"

/*************************************************************
 * Overview
 *  Left rotation plus
 *    OMNI_4_P        OMNI_4_F
 *      1             1    0
 *   2     0
 *      3             2    3
 *
 *    OMNI_3_P       OMNI_3_F
 *      0            1     0
 *
 *   1     2            2
*************************************************************/

typedef enum {
	OMNI_4_F = 4,
	OMNI_4_P = -4,
	OMNI_3_F = 3,
	OMNI_3_P = -3
} tOMNITYPE;

extern void InitOmni (tOMNITYPE type, float WheelFromCenterOfMachine_meter, float Wheel_meter);

extern void CalcOmni (float freq[], float vx, float vy, float omega_rps, float theta);

#endif /* OMNI_H_ */

#ifndef ODOMETRY_H_
#define ODOMETRY_H_
#include "Encoder.h"
#include "Gyro.h"

#if 0
// Global coordinates
	+gy
-gx		+gx
	-gy
gr:
g
// Object coordinates (machine)
	+oby
-obx		+obx
	-oby
obr

#endif
typedef struct {
	uint8_t     qGyroFLG;
	float      qGlb_Vx;
	float      qGlb_Vy;
	float      qGlb_x;
	float      qGlb_y;
	float      qObj_Vx;
	float      qObj_Vy;
	float      qCnvParm;
	float      qTs;				// sec
	tEncParm    *pEncX;
	tEncParm    *pEncY;
	tGyroParm   *pGyro;
} tOdometryParm;

tOdometryParm hodometry;

extern void InitOdometry (tOdometryParm *pParm, tEncParm *pEncParmX, tEncParm *pEncParmY, tGyroParm *pGyroParm, float ts_ms, float WheelDiameter);

extern void setOriginPosition (tOdometryParm *pParm, float x, float y);

extern void CalcMachinePosition(tOdometryParm* pParm);

#endif /* ODOMETRY_H_ */

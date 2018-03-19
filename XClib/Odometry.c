#include "math.h"
#include "Odometry.h"

void InitOdometry (tOdometryParm *pParm, tEncParm *pEncParmX, tEncParm *pEncParmY, tGyroParm *pGyroParm, float ts_ms, float WheelDiameter) {
	pParm->qGyroFLG = 0;
	pParm->pEncX = pEncParmX;
	pParm->pEncY = pEncParmY;
	pParm->pGyro = pGyroParm;
	pParm->qCnvParm = WheelDiameter / 2.0;
	pParm->qTs = ts_ms / 1000.0;
	pParm->qObj_Vx = 0;
	pParm->qObj_Vx = 0;
	pParm->qGlb_Vx = 0;
	pParm->qGlb_Vy = 0;
	pParm->qGlb_x  = 0;
	pParm->qGlb_y  = 0;
}

void setOriginPosition (tOdometryParm *pParm, float x, float y) {
	pParm->qObj_Vx = 0;
	pParm->qObj_Vx = 0;
	pParm->qGlb_Vx = 0;
	pParm->qGlb_Vy = 0;
	pParm->qGlb_x  = x;
	pParm->qGlb_y  = y;
}

void CalcMachinePosition(tOdometryParm *pParm){
	float _cos = cos(pParm->pGyro->qRad);
	float _sin = sin(pParm->pGyro->qRad);

	CalcENC(pParm->pEncX);
	CalcENC(pParm->pEncY);

	pParm->qObj_Vx = pParm->pEncX->qOmega * pParm->qCnvParm;// [rad/s] * d / 2.0
	pParm->qObj_Vy = pParm->pEncY->qOmega * pParm->qCnvParm;
	pParm->qGlb_Vx = pParm->qObj_Vx * _cos + pParm->qObj_Vy * -_sin;
	pParm->qGlb_Vy = pParm->qObj_Vx * _sin + pParm->qObj_Vy * _cos;
	pParm->qGlb_x += pParm->qGlb_Vx * pParm->qTs;
	pParm->qGlb_y += pParm->qGlb_Vy * pParm->qTs;
}

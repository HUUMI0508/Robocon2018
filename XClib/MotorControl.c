#include "MotorControl.h"
#include "PIcontrol.h"
#include "Encoder.h"

void InitMCTRL(tMCTRL *mtr, tEncParm *encParm, float vel_limit) {
	mtr->qDuty = 0;
	mtr->qFreq = 0.0;
	mtr->pEncParm = encParm;
	mtr->vel_max = vel_limit;
	mtr->vel_min = -vel_limit;
	InitPI(&mtr->pPIParm, 0, 0, 24, -24, 1);
}

void setPGainCalc(tMCTRL *c, float tim_const_ms) {
	c->qKp = 1000.0 / tim_const_ms;
}

void setPIGainCalc(tMCTRL *c, float tim_const_ms, uint16_t rpv, uint16_t wn, float zeta) {
	float trans_const = (rpv / 60.0) * c->pEncParm->qGm;

	c->pPIParm.qKp = (2.0 * zeta * wn * (tim_const_ms / 1000) - 1.0) / trans_const;
	c->pPIParm.qKi = ((tim_const_ms / 1000) * (wn * wn)) / trans_const;
	CalcPIconst(&c->pPIParm);
}

void CalcPosVel(tMCTRL *c, float u) {
	c->qFreq = c->qKp * u;
	if (c->qFreq > c->vel_max) {
		c->qFreq = c->vel_max;
	} else if (c->qFreq < c->vel_min) {
		c->qFreq = c->vel_min;
	}
}

void CalcVelPWM(tMCTRL *c, float u) {
	int16_t temp;

	CalcPI(&c->pPIParm, u);
	temp = (int16_t) (c->pPIParm.qOut / 24.0 * MAX_DUTY);
	if (-20 < temp && temp < 20) {
		c->qDuty = 0;
	} else {
		c->qDuty = temp;
	}
}

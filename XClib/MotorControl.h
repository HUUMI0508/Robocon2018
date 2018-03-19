#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_
#include "PIcontrol.h"
#include "Encoder.h"

typedef struct {
	float qRev;//ref
	float qFreq;//ref
	int16_t qDuty;//ref
	int16_t qDuty_past;
	float vel_max;
	float vel_min;
	float qKp;//position
	tPIParm pPIParm;//velocity
	tEncParm *pEncParm;
} tMCTRL;

tMCTRL hmtr1;
tMCTRL hmtr2;
tMCTRL hmtr3;
tMCTRL hmtr4;

#define MAX_DUTY 1000

extern void InitMCTRL(tMCTRL *mtr, tEncParm *encParm, float vel_limit);
/**
 * @brief calculate pi gain from each parameter
 * @param tim_const		time constant[ms]
 * @param rpv			Kv[rpm/V]// Revoltion constant rev/Volt
 * @param wn			Specific frequency[rad/s]
 * @param zeta			Damping coefficient
 */
extern void setPGainCalc(tMCTRL *c, float tim_const_ms);

extern void setPIGainCalc(tMCTRL *c, float tim_const_ms, uint16_t rpv, uint16_t wn, float zeta);

extern void CalcPosVel(tMCTRL *c, float u);
/**
 * @brief calculate duty use pi control from encoder velocity
 * @note
 * @param *c
 */
extern void CalcVelPWM(tMCTRL *c, float u);
#endif /* MOTORCONTROL_H_ */

#ifndef USER_H_
#define USER_H_
#include "GANMO.h"
#include "../Inc/main.h"
#define ARM_MATH_CM7
#include "arm_math.h"
#include "arm_const_structs.h"
//#include "User.h"
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
#include "manzi_v2.h"
//#include "manzi.c"
#include "position.h"

/*** User Global***/
tMD md1;
tMD md2;
tMD md3;
tMD md4;


#if 0
//increase speed 15.625 times
#define Gm 9.0
#define Gm1 37.0
#define Gm1_ 16.0
#define Gt1 19.0
#define Gm1__ 23.0
#define Ge1 38.0
#define GEAR_RATIO_ACH	(1.0/((Gm/Gm1)*(Gm1_/Gt1)))
#define PPR_ACH			(((Gm/Gm1)*(Gm1__/Ge1))*500.0)
#define T_CONSTRPV_ACH	35
#define RPV_ACH			317
#define OMEGA_ACH		20
#define ZETA_ACH		1.0
// BCH
#define GEAR_RATIO_BCH	(1.0/((Gm/Gm1)*(Gm1_/Gt1)))
#define PPR_BCH			(((Gm/Gm1)*(Gm1__/Ge1))*500.0)
#define T_CONSTRPV_BCH	35
#define RPV_BCH			317
#define OMEGA_BCH		20
#define ZETA_BCH		1.0

#elif 0 // kasoku souchi 775
#define G1 26.0
#define G2 32.0
// ACH
#define GEAR_RATIO_ACH	(G1*G2/G1)
#define PPR_ACH			((1.0/(G1*G2/G1))*100.0)
#define T_CONSTRPV_ACH	35
#define RPV_ACH			1000
#define OMEGA_ACH		20
#define ZETA_ACH		1.0
#define encderach		COUNTERCLOCKWISE
// BCH
#define GEAR_RATIO_BCH	(G1*G2/G1)
#define PPR_BCH			((1.0/(G1*G2/G1))*200.0)
#define T_CONSTRPV_BCH	35
#define RPV_BCH			1000
#define OMEGA_BCH		20
#define ZETA_BCH		1.0
#define encderbch		COUNTERCLOCKWISE

#elif 0
// ACH
#define GEAR_RATIO_Gm_ACH	1/72.0
#define GEAR_RATIO_Ge_ACH	1
#define PPR_ACH			500
#define T_CONSTRPV_ACH	35.
#define RPV_ACH			317
#define OMEGA_ACH		20
#define ZETA_ACH		1.0
//extern tDIRECTION ENC_ACH = CLOCKWISE;
// BCH
#define GEAR_RATIO_Gm_BCH	1/26.0
#define GEAR_RATIO_Ge_BCH	1
#define PPR_BCH			100
#define T_CONSTRPV_BCH	35
#define RPV_BCH			317
#define OMEGA_BCH		20
#define ZETA_BCH		1.0
//extern tDIRECTION ENC_BCH = CLOCKWISE;

#elif 1 // maxson junsei
// ACH
#define GEAR_RATIO_Gm_ACH	1/39.0
#define GEAR_RATIO_Ge_ACH	1
#define PPR_ACH				100
#define T_CONSTRPV_ACH		35
#define RPV_ACH				317
#define OMEGA_ACH			20
#define ZETA_ACH			1.0
//tDIRECTION ENC_ACH = CLOCKWISE;
// BCH
#define GEAR_RATIO_Gm_BCH	1/39.0
#define GEAR_RATIO_Ge_BCH	1
#define PPR_BCH				100
#define T_CONSTRPV_BCH		35
#define RPV_BCH				317
#define OMEGA_BCH			20
#define ZETA_BCH			1.0
//tDIRECTION ENC_BCH = CLOCKWISE;
#endif

FLG LINEAR_MOVE;
FLG ROTATION_MOVE;
FLG CONTROLL_START;

typedef enum{
	START,
	INIT,
	CATCH_POSITION,
	CATCH_SHUTLE,
	THROW_READY,
	FIRE,
	END,
}PAISEN;
PAISEN paisen;
FLG INIT_FINISH;

typedef enum{
	Z1,
	Z2,
	Z3
}THROW_ZONE;
THROW_ZONE throw_zone;

FLG CATCH;
FLG READY;
FLG THROW;

extern void task_1(void);
extern void InitUSER(void);
extern void ms1(void);
extern void ms50(void);
extern void PAISEN_SEQUENCE(void);

double c_position[3][2];

#endif /* USER_H_ */

/*
 * manzi_v2.h
 *
 *  Created on: 2018/03/12
 *      Author: RoboPro2015
 */

#ifndef MANZI_V2_H_
#define MANZI_V2_H_


#include "GANMO.h"

/*
//根本用モータパラ
#define ROOT_MAX_SPEED_DEG		-720.		//deg/s//1151
#define ROOT_ACCEL_TIME		(((180 * 2) / ROOT_MAX_SPEED_DEG) * 1000.)	//ms
#define ROOT_MAX_SPEED_RPS 	(ROOT_MAX_SPEED_DEG / 360.)	//rps

#define ROOT_RETURN_SPEED_DEG	180.		//deg/s//1151
#define ROOT_RETURN_SPEED_RPS 	(ROOT_RETURN_SPEED_DEG / 360.)	//rps
#define	ROOT_CONTROLL_TIME		1.	//ms 制御周期
//#define ROOT_SPEED_DELTA	((ROOT_MAX_SPEED_RPS / ROOT_ACCEL_TIME) * ROOT_CONTROLL_TIME)	//一更新あたりの上昇速度
#define ROOT_LAST_ANGLE_DEG		-180.//deg
#define ROOT_LAST_ANGLE		(ROOT_LAST_ANGLE_DEG / 360.)


#define TIP_ACCEL_TIME		(((180 * 2) / ROOT_MAX_SPEED_DEG) * 1000.)		//ms
//#define TIP_MAX_SPEED_DEG	((270 * 2) / (TIP_ACCEL_TIME / 1000))		//deg/s
#define TIP_MAX_SPEED_DEG	((360 * 2) / (TIP_ACCEL_TIME / 1000.))		//deg/s
#define TIP_MAX_SPEED_RPS 	(TIP_MAX_SPEED_DEG / 360.)	//rps

#define TIP_RETURN_SPEED_DEG		360.		//deg/s
#define TIP_RETURN_SPEED_RPS 	(TIP_RETURN_SPEED_DEG / 360.)	//rps
#define	TIP_CONTROLL_TIME		1.	//ms 制御周期
//#define TIP_SPEED_DELTA		((TIP_MAX_SPEED_RPS / TIP_ACCEL_TIME) * TIP_CONTROLL_TIME)	//一更新あたりの上昇速度
#define TIP_LAST_ANGLE_DEG		-360.//deg
#define TIP_LAST_ANGLE		(TIP_LAST_ANGLE_DEG / 360.)
*/
/*
 * 操作方法
 * 下でシャトルコックをつかむ
 * 右で発射位置へ
 * △でzone1のパラメータ
 * 〇でzone2のパラメータ
 * R3でzone3のパラメータをそれぞれセット
 * 上で射出
 * 以下ループ
 */
#define ROOT_GOLDEN_SPEED		-565.//-575.//-575.		//deg/s//1151
#define ROOT_GOLDEN_DEG		-180.//deg
#define TIP_GOLDEN_DEG		-360.//deg

#define ROOT_Z1_SPEED	-395.//-390.//-400.//ok
#define ROOT_Z1_DEG		-180.
#define TIP_Z1_DEG	-360.

#define ROOT_Z2_SPEED	-460.//ok
#define ROOT_Z2_DEG		-180.
#define TIP_Z2_DEG	-360.

#define ROOT_RETURN_SPEED_DEG	180.		//deg/s//1151

#define RELEASE_Z1 250.
#define RELEASE_Z2 240.
#define RELEASE_GOLDEN 228.

#define UKETORI_ROOT 	160.
#define UKETORI_TIP		60.


double RELEASE_ANGLE_DEG;
double RELEASE_ANGLE;

typedef struct{
	double Speed_Deg;
	double Accel_Time;
	double Speed_Rps;
	double Last_Angle_Deg;
	double Last_Angle_R;
}sParm;
sParm sRoot;
sParm sTip;

typedef struct{
	double Speed;
	double Speed_Old;
	double Angle;
	double Move_Angle;
	double Now_Angle;
	double Goal_Angle;
	double Limit_Speed;
	double Speed_Delta;
	//double R_Speed_Delta;
}mParm;

mParm pRoot;
mParm pTip;

typedef struct{
	double Move_Angle;
	//double Now_Angle;
	double Goal_Angle;
	double Speed_Delta;
	double Accel_Time;
}dParm;

dParm dRoot;
dParm dTip;

typedef struct{
	int Lag_Count;
}PROTECT;
PROTECT protect;
#define PROTECT_TIME 100.
//static int Lag_Count = 1000;

/*
typedef enum{
	DEFAULT,
	CROSS,
	SQUARE,
	TRIANGLE,
	CIRCLE,

	UP,
	LEFT,
	RIGHT,
	DOWN,
	L1,
	R1,
	R2
}BUTTON;
*/
typedef struct{
	//FLG DEFAULT;
	FLG CROSS;
	FLG SQUARE;
	FLG TRIANGLE;
	FLG CIRCLE;

	FLG R1;
	FLG R2;
	FLG R3;
	FLG L1;
	FLG L2;
	FLG L3;

	FLG UP;
	FLG LEFT;
	FLG RIGHT;
	FLG DOWN;

	FLG ZONE1;
	FLG ZONE2;
	FLG ZONE3;
}COMMAND;
COMMAND command;
FLG Fire;
FLG Down;
FLG ROOT_D;
FLG TIP_D;
FLG ROOT_DD;
FLG TIP_DD;
//BUTTON Button;

GPIO_PinState photo1;
GPIO_PinState photo2;

extern void MANZI_INIT();
extern void MOTOR_CONTROLL();
extern void SPEED_DOWN();

extern void INPUT();
extern void PARM_SET(mParm *c,double Limit_S, double Last_A);
extern void SPEED_SET(double Root_Speed, double Root_Angle_Deg, double Tip_Angle_Deg);
extern void FIRE_FLAG_M();
#endif /* MANZI_V2_H_ */

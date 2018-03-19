/*
 * manzi_v2.h
 *
 *  Created on: 2018/03/12
 *      Author: RoboPro2015
 */

#ifndef MANZI_V2_H_
#define MANZI_V2_H_


#include "GANMO.h"


//根本用モータパラ
#define ROOT_MAX_SPEED_DEG		-540.		//deg/s//1151
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



#define ROOT_GET_MAX_SPEED_DEG		-180.		//deg/s//1151
#define ROOT_GET_MAX_SPEED_RPS 	(ROOT_GET_MAX_SPEED_DEG / 360.)	//rps
#define ROOT_GET_LAST_ANGLE_DEG		-270.//deg
#define ROOT_GET_LAST_ANGLE		(ROOT_GET_LAST_ANGLE_DEG / 360.)

//#define TIP_ACCEL_TIME		(((180 * 2) / ROOT_GET_MAX_SPEED_DEG) * 1000.)		//ms
#define TIP_GET_MAX_SPEED_DEG	180//((360 * 2) / (TIP_ACCEL_TIME / 1000.))		//deg/s
#define TIP_GET_MAX_SPEED_RPS 	(TIP_GET_MAX_SPEED_DEG / 360.)	//rps
#define TIP_GET_LAST_ANGLE_DEG		0.//deg
#define TIP_GET_LAST_ANGLE		(TIP_GET_LAST_ANGLE_DEG / 360.)


double RELEASE_ANGLE_DEG;
double RELEASE_ANGLE;

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
	int Lag_Count;
}PROTECT;
PROTECT protect;
#define PROTECT_TIME 100.
//static int Lag_Count = 1000;

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

FLG Fire;
FLG ROOT_D;
FLG TIP_D;
BUTTON Button;


extern void MANZI_INIT();
extern void MOTOR_CONTROLL();
extern void INPUT();
extern void PARM_SET(mParm *c,double Limit_S, double Last_A);
#endif /* MANZI_V2_H_ */

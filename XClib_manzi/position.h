/*
 * position.h
 *
 *  Created on: 2018/02/14
 *      Author: RoboPro2015
 */

#ifndef POSITION_H_
#define POSITION_H_
#include "GANMO.h"
#include "User.h"

#define ROOT_ARM 0.295
#define TIP_ARM 0.375
#define P_GAIN 2.

typedef struct{
	double Inref;
	double Inref_P;
	double P_Error;
	double P_Gain;
	double Limit;
}CONTROLL;
CONTROLL con1;
CONTROLL con2;

typedef struct{
	double Now_X_Point;//-0.3;
	double Now_Y_Point;//0.0;

	double Ref_X_Point;//-0.3;
	double Ref_Y_Point;//-0.3;

	double Tip_Length;
	double Tip_Angle;
	double Tip_Angle_Deg;
	double Tip_Length_Ref;
	double Tip_Angle_Ref;
	double Tip_Angle_Deg_Ref;

	double Angle_Arm1;//(50 * PI()) / 180;
	double Angle_Arm2;//(50 * PI()) / 180;

	double Step_X;
	double Step_Y;

	double Moving_X_Point;//Now_X_Point;
	double Moving_Y_Point;//Now_Y_Point;

	//double X;
	//double Y;

	long int time;//1000; //ms

}LINEAR;
LINEAR linear;

typedef struct{
	double Now_Root_Angle;
	double Now_Tip_Angle;

	double Ref_Root_Angle;
	double Ref_Tip_Angle;

	double Step_Root;
	double Step_Tip;

	double time;
}ROTATION;
ROTATION rotation;
typedef enum{
	SET_LINEAR,
	SET_ROTATION
}SET_SELECT;
SET_SELECT set_select;

typedef enum {
	POSITION,
	SPEED,
	STOP
} MODE;
MODE PS_MODE;

typedef enum{
	cSTOP,
	cMIDSTREAM,
	cEND
}C_SEQUENCE;
C_SEQUENCE c_sequence;
C_SEQUENCE c_sequence2;

typedef enum{
	oSTOP,
	MOVE_ROOT,
	SET_ROOT,
	MOVE_TIP,
	SET_TIP
}ORIGIN;
ORIGIN origin;
FLG oflg;

typedef struct{
	double Angle_Deg;
	double Angle_R;
}OFFSET;
OFFSET offset_root;
OFFSET offset_tip;

FLG flg;
/*
static double Inref_1 = 0.;
static double Inref_2 = 0.;
static double Inref_P_1 = 0.;
static double Inref_P_2 = 0.;
static double P_Error_1 = 0.;
static double P_Error_2 = 0.;
//static double P_Out = 0.;
static double P_Gain_1 = 1.0;
static double P_Gain_2 = 1.0;
*/

extern void POSITION_INIT();
extern void POSITION_CONTROLL();
extern void LINEAR_INIT();
extern void ROTATION_INIT();
extern void SET_NOW_POSITION();
extern void SET_REF_LINEAR(double REF_X, double REF_Y, long int time);
extern void LINEAR_ORBIT();
extern void SET_REF_ROTATION(double REF_ROOT, double REF_TIP, double time);
extern void ROTATION_ORBIT();
extern void CATCH_SEQUENCE();
extern void SET_STARTING_POINT();
#endif /* POSITION_H_ */

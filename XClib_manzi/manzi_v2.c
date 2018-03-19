/*
 * manzi_v2.c
 *
 *  Created on: 2018/03/12
 *      Author: RoboPro2015
 */


#include "stdlib.h"
#include "manzi_v2.h"
#include "position.h"


void MANZI_INIT(){
	RELEASE_ANGLE_DEG = 225;
	RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

	pRoot.Angle = 0;
	pRoot.Goal_Angle = 0;
	pRoot.Limit_Speed = 0;
	pRoot.Move_Angle = 0;
	pRoot.Now_Angle = 0;
	pRoot.Speed = 0;
	pRoot.Speed_Old = 0;
	//pRoot.R_Speed_Delta = ((ROOT_RETURN_SPEED_RPS / fabs(ROOT_ACCEL_TIME)) * ROOT_CONTROLL_TIME);

	pTip.Angle = 0;
	pTip.Goal_Angle = 0;
	pTip.Limit_Speed = 0;
	pTip.Move_Angle = 0;
	pTip.Now_Angle = 0;
	pTip.Speed = 0;
	pTip.Speed_Old = 0;
	//pTip.R_Speed_Delta = ((TIP_MAX_SPEED_RPS / fabs(TIP_ACCEL_TIME)) * TIP_CONTROLL_TIME);

	protect.Lag_Count = PROTECT_TIME;
	Fire = NACT;
	ROOT_D = ACT;
	TIP_D = ACT;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);


}

void MOTOR_CONTROLL(){
	if(Fire == ACT){
		//�l�X�V
		pRoot.Speed_Old = pRoot.Speed;
		pTip.Speed_Old = pTip.Speed;

		if(ROOT_D == ACT){
			//���x���Z
			pRoot.Speed += pRoot.Speed_Delta;
			//�p�x����
			pRoot.Angle = henc1.qDeg / 360.;
			//���x����
			if(fabs(pRoot.Speed) >= fabs(pRoot.Limit_Speed)) pRoot.Speed = pRoot.Limit_Speed;

			con1.Inref = pRoot.Speed;

			if(fabs(pRoot.Angle - pRoot.Now_Angle) >= fabs(pRoot.Move_Angle)){
				pRoot.Now_Angle = pRoot.Goal_Angle;
				con1.Inref_P = (pRoot.Now_Angle * 360.);
				con1.Inref = 0;
				pRoot.Speed = 0;
				pRoot.Speed_Old = 0;
				ROOT_D = NACT;
			}
		}
		if(TIP_D == ACT){
			pTip.Speed += pTip.Speed_Delta;
			pTip.Angle = henc2.qDeg / 360.;
			if(fabs(pTip.Speed) >= fabs(pTip.Limit_Speed)) pTip.Speed = pTip.Limit_Speed;

			con2.Inref = (pTip.Speed - pRoot.Speed);

			//if(henc3.qDeg >= TIP_LAST_ANGLE_DEG)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			if(fabs((pTip.Angle + pRoot.Angle) - pTip.Now_Angle) >= fabs(RELEASE_ANGLE)){
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
			}
			if(fabs((pTip.Angle + pRoot.Angle) - pTip.Now_Angle) >= fabs(pTip.Move_Angle)){
				pTip.Now_Angle = pTip.Goal_Angle;
				con2.Inref_P = ((pTip.Now_Angle - pRoot.Goal_Angle) * 360);
				con2.Inref = 0;
				pTip.Speed = 0;
				pTip.Speed_Old = 0;
				TIP_D = NACT;
			}

		}
		if((TIP_D == NACT) && (ROOT_D == NACT)){
			LED_ORANGE_T();
			protect.Lag_Count = 0;
			con1.P_Gain = 0;
			con2.P_Gain = 0;
			PS_MODE = STOP;
			Fire = NACT;
		}
	}
}


FLG F_Button = NACT;
void INPUT(){
	//SW1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);//B2
	//SW2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);//E9
	//�ˏo
		if(hDS.BUTTON.CIRCLE){
			LED_GREEN_T();
			Button = CIRCLE;
			/*
			PARM_SET(&pRoot,ROOT_MAX_SPEED_RPS,ROOT_LAST_ANGLE);
			PARM_SET(&pTip,TIP_MAX_SPEED_RPS,TIP_LAST_ANGLE);
			*/
			//Root����v�Z Root�͑�4������TRUE��
			PARM_SET(&pRoot,ROOT_MAX_SPEED_DEG,ROOT_LAST_ANGLE_DEG,1);
			PARM_SET(&pTip,TIP_MAX_SPEED_DEG,TIP_LAST_ANGLE_DEG,0);
			/*
			pRoot.Limit_Speed = ROOT_MAX_SPEED_RPS;
			pRoot.Move_Angle = ROOT_LAST_ANGLE - pRoot.Now_Angle;
			pRoot.Goal_Angle = ROOT_LAST_ANGLE;
			pRoot.Speed_Delta = ((ROOT_MAX_SPEED_RPS / fabs(ROOT_ACCEL_TIME)) * ROOT_CONTROLL_TIME);
//absf();
			pTip.Limit_Speed = TIP_MAX_SPEED_RPS;
			pTip.Move_Angle = TIP_LAST_ANGLE - pTip.Now_Angle;
			pTip.Goal_Angle = TIP_LAST_ANGLE;
			pTip.Speed_Delta = ((TIP_MAX_SPEED_RPS / fabs(TIP_ACCEL_TIME)) * TIP_CONTROLL_TIME);
			*/
			//if(pRoot.Move_Angle != 0){
				if(F_Button == NACT){
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
					ROOT_D = ACT;
					TIP_D = ACT;
					PS_MODE = SPEED;///
					Fire = ACT;
					F_Button = ACT;
				}
			//}
		}
		//�߂�
		else if(hDS.BUTTON.SQUARE){
			LED_GREEN_T();
			Button = SQUARE;
			PARM_SET(&pRoot,ROOT_RETURN_SPEED_RPS,0,1);
			PARM_SET(&pTip,TIP_RETURN_SPEED_RPS,0,0);
			/*
			pRoot.Limit_Speed = ROOT_RETURN_SPEED_RPS;
			pRoot.Move_Angle = 0 - pRoot.Now_Angle;
			pRoot.Goal_Angle = 0;
			pRoot.Speed_Delta = ((ROOT_RETURN_SPEED_RPS / fabs(ROOT_ACCEL_TIME)) * ROOT_CONTROLL_TIME);

			pTip.Limit_Speed = TIP_RETURN_SPEED_RPS;
			pTip.Move_Angle = 0 - pTip.Now_Angle;
			pTip.Goal_Angle = 0;
			pTip.Speed_Delta = ((TIP_RETURN_SPEED_RPS / fabs(TIP_ACCEL_TIME)) * TIP_CONTROLL_TIME);
			*/
			//if(pRoot.Move_Angle != 0){
				if(F_Button == NACT){
					ROOT_D = ACT;
					TIP_D = ACT;
					PS_MODE = SPEED;///
					Fire = ACT;
					F_Button = ACT;
				}
			//}
		}
		//���WB
		else if(hDS.BUTTON.TRIANGLE){
			LED_GREEN_T();
			Button = CIRCLE;
			PARM_SET(&pRoot,ROOT_GET_MAX_SPEED_RPS,ROOT_GET_LAST_ANGLE,1);
			PARM_SET(&pTip,TIP_GET_MAX_SPEED_RPS,TIP_GET_LAST_ANGLE,0);
			if(F_Button == NACT){
				ROOT_D = ACT;
				TIP_D = ACT;
				PS_MODE = SPEED;///
				Fire = ACT;
				F_Button = ACT;
			}
		}
		else if(hDS.BUTTON.L1){
			if(F_Button == NACT){
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);//�d����3 OFF
				F_Button = ACT;
			}
		}
		else if(hDS.BUTTON.L2){
			if(F_Button == NACT){
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);//�d����3 OFF
				F_Button = ACT;
			}
		}
		else if(hDS.BUTTON.CROSS){
			LED_ORANGE_T();
			protect.Lag_Count = 0;
			con1.P_Gain = 0;
			con2.P_Gain = 0;
			PS_MODE = STOP;
			Fire = NACT;

		}
		/*
		else if(hPS.BUTTON.R1){
			SET_NOW_POSITION();
			SET_REF(0.3, 0.3,1000);
			LINEAR_MOVE = ACT;
			LINEAR_ORBIT();
		}
		else if(hPS.BUTTON.R2){
			SET_NOW_POSITION();
			SET_REF(0.67, 0.0,1000);
			LINEAR_MOVE = ACT;
			LINEAR_ORBIT();
		}
		*/
		else{
			if(F_Button == ACT) F_Button = NACT;
		}
}

void PARM_SET(mParm *c,double Limit_S, double Last_A, uint8_t Flg){
	double Limit_Speed_RPS = Limit_S / 360.;//rps
	double Last_Angle_R = Last_A / 360.;//rotation
	static double Accel_Time = 0;//�������� Root�
	if(Flg == 1) Accel_Time = (((180 * 2) / Limit_S) * 1000.);

	c->Move_Angle = Last_Angle_R - c->Now_Angle;//�ړ��p
	c->Goal_Angle = Last_Angle_R;//�ŏI�p

	if(c->Move_Angle > 0)c->Limit_Speed = Limit_Speed_RPS;//���E���x	��Βl
	else c->Limit_Speed = -Limit_Speed_RPS;

	c->Speed_Delta = ((c->Limit_Speed / fabs(Accel_Time)));
}

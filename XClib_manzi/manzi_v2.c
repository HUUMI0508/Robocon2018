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

	command.CROSS = NACT;
	command.SQUARE = NACT;
	command.TRIANGLE = NACT;
	command.CIRCLE = NACT;

	command.R1 = NACT;
	command.R2 = NACT;
	command.R3 = NACT;
	command.L1 = NACT;
	command.L2 = NACT;
	command.L3 = NACT;

	command.UP = NACT;
	command.LEFT = NACT;
	command.RIGHT = NACT;
	command.DOWN = NACT;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);

}

void MOTOR_CONTROLL(){
	if(Fire == ACT){
		//値更新
		pRoot.Speed_Old = pRoot.Speed;
		pTip.Speed_Old = pTip.Speed;

		if(ROOT_D == ACT){
			//速度加算
			pRoot.Speed += pRoot.Speed_Delta;
			//角度崇徳
			pRoot.Angle = (henc1.qDeg / 360.) - offset_root.Angle_R;
			//速度制限
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
			pTip.Angle = (henc2.qDeg / 360.) - offset_tip.Angle_R;
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
			THROW = NACT;
			RELOAD = NACT;
			PS_MODE = STOP;
			Fire = NACT;
		}
	}
}


FLG F_Button = NACT;
void INPUT(){
	//SW1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);//B2
	//SW2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);//E9
	//射出
	if(hDS.BUTTON.CROSS)command.CROSS = ACT;
	else command.CROSS = NACT;
	if(hDS.BUTTON.SQUARE)command.SQUARE = ACT;
	else command.SQUARE = NACT;
	if(hDS.BUTTON.TRIANGLE)command.TRIANGLE = ACT;
	else command.TRIANGLE = NACT;
	if(hDS.BUTTON.CIRCLE)command.CIRCLE = ACT;
	else command.CIRCLE = NACT;

	if(hDS.BUTTON.L1)command.L1 = ACT;
	else command.L1 = NACT;
	if(hDS.BUTTON.L2)command.L2 = ACT;
	else command.L2 = NACT;
	if(hDS.BUTTON.L3)command.L3 = ACT;
	else command.L3 = NACT;
	if(hDS.BUTTON.R1)command.R1 = ACT;
	else command.R1 = NACT;
	if(hDS.BUTTON.R2)command.R2 = ACT;
	else command.R2 = NACT;

	if(hDS.BUTTON.UP)command.UP = ACT;
	else command.UP = NACT;
	if(hDS.BUTTON.LEFT)command.LEFT = ACT;
	else command.LEFT = NACT;
	if(hDS.BUTTON.RIGHT)command.RIGHT = ACT;
	else command.RIGHT = NACT;
	if(hDS.BUTTON.DOWN)command.DOWN = ACT;
	else command.DOWN = NACT;

#if 0
		if(hDS.BUTTON.CIRCLE){//GOLDEN
			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_GOLDEN_SPEED, ROOT_GOLDEN_DEG, TIP_GOLDEN_DEG);
				if(F_Button == NACT){
					//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
					ROOT_D = ACT;
					TIP_D = ACT;
					PS_MODE = SPEED;///
					Fire = ACT;
					F_Button = ACT;
				}
		}
		//戻り
		else if(hDS.BUTTON.SQUARE){//RETURN
			LED_GREEN_T();
			//Button = SQUARE;

			RELEASE_ANGLE_DEG = 9999.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_RETURN_SPEED_DEG, 0, 0);
				if(F_Button == NACT){
					ROOT_D = ACT;
					TIP_D = ACT;
					PS_MODE = SPEED;///
					Fire = ACT;
					F_Button = ACT;
				}
		}
		//座標B
		else if(hDS.BUTTON.TRIANGLE){//ZONE1
			LED_GREEN_T();

			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_Z1_SPEED, ROOT_Z1_DEG, TIP_Z1_DEG );
				if(F_Button == NACT){
					ROOT_D = ACT;
					TIP_D = ACT;
					PS_MODE = SPEED;///
					Fire = ACT;
					F_Button = ACT;
				}
		}
		else if(hDS.BUTTON.CROSS){//ZONE2
			LED_GREEN_T();

			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_Z2_SPEED, ROOT_Z2_DEG, TIP_Z2_DEG);
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
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);//電磁弁3 OFF
				F_Button = ACT;
			}
		}

		else if(hDS.BUTTON.L2){
			if(F_Button == NACT){
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);//電磁弁3 OFF
				F_Button = ACT;
			}
		}

		else if(hDS.BUTTON.R1){//受け取り位置へ移動
			if(F_Button == NACT){
				double data[2][3] = {{0.,150.,1000.},
									 {160.,40.,1000.}};
				for(int i = 0; i <= 1; i++){
					for(int j = 0; j <= 2; j++){
						c_position[i][j] = data[i][j];
					}
				}
				c_sequence = cMIDSTREAM;
				F_Button = ACT;
			}
		}
		else if(hDS.BUTTON.R2){//初期位置へ戻る
			if(F_Button == NACT){
				double data[2][3] = {{0.,150.,1000.},
									 {0.,0.,1000.}};
				for(int i = 0; i <= 1; i++){
					for(int j = 0; j <= 2; j++){
						c_position[i][j] = data[i][j];
					}
				}
				c_sequence = cMIDSTREAM;
				F_Button = ACT;
			}
			/*
			if(F_Button == NACT){
				set_select = SET_ROTATION;
				SET_NOW_POSITION();
				SET_REF_ROTATION(160, 40, 1000.);
				ROTATION_MOVE = ACT;
				F_Button = ACT;
			}
			*/
		}

		else{
			if(F_Button == ACT) F_Button = NACT;
		}
#elif 1
		if(command.CROSS == ACT){//ZONE2
			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_Z2_SPEED, ROOT_Z2_DEG, TIP_Z2_DEG);

			FIRE_FLAG_M();

			command.CROSS = NACT;
		}
		if(command.SQUARE == ACT){//RETURN
			RELEASE_ANGLE_DEG = 9999.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_RETURN_SPEED_DEG, 0, 0);

			FIRE_FLAG_M();

			command.SQUARE = NACT;
		}
		if(command.TRIANGLE == ACT){//ZONE1
			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_Z1_SPEED, ROOT_Z1_DEG, TIP_Z1_DEG );

			FIRE_FLAG_M();

			command.TRIANGLE = NACT;
		}
		if(command.CIRCLE == ACT){//GOLDEN
			RELEASE_ANGLE_DEG = 225.;
			RELEASE_ANGLE = (RELEASE_ANGLE_DEG / 360.);

			SPEED_SET(ROOT_GOLDEN_SPEED, ROOT_GOLDEN_DEG, TIP_GOLDEN_DEG);

			FIRE_FLAG_M();

			command.CIRCLE = NACT;
		}


		if(command.L1 == ACT){
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);//電磁弁3 OFF
			command.L1 = NACT;
		}
		if(command.L2== ACT){
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);//電磁弁3 OFF
			command.L2 = NACT;
		}
		if(command.L3 == ACT){
			//origin = MOVE_ROOT;
			paisen = INIT;
			command.L3 = NACT;
		}
		if(command.R1 == ACT){//受け取り位置へ移動
			double data[2][3] = {{0.,150.,1000.},
								 {160.,40.,1000.}};
			for(int i = 0; i <= 1; i++){
				for(int j = 0; j <= 2; j++){
					c_position[i][j] = data[i][j];
				}
			}
			c_sequence = cMIDSTREAM;
			command.R1 = NACT;
		}
		if(command.R2 == ACT){//初期位置へ戻る
			double data[2][3] = {{0.,150.,1000.},
								 {0.,0.,1000.}};
			for(int i = 0; i <= 1; i++){
				for(int j = 0; j <= 2; j++){
					c_position[i][j] = data[i][j];
				}
			}
			c_sequence = cMIDSTREAM;
			command.R2 = NACT;
		}
		if(command.R3 == ACT){//つかむ
			//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);//電磁弁3 OFF
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
			command.R3 = NACT;
		}


		if(command.UP == ACT){
			command.UP = NACT;
		}
		if(command.LEFT == ACT){
			command.LEFT = NACT;
		}
		if(command.RIGHT == ACT){
			command.RIGHT = NACT;
		}
		if(command.DOWN == ACT){
			command.DOWN = NACT;
		}
#endif
}

void PARM_SET(mParm *c,double Limit_S, double Last_A){
	c->Limit_Speed = Limit_S;//限界速度
	c->Move_Angle = Last_A - c->Now_Angle;//移動角
	c->Goal_Angle = Last_A;//最終角
	c->Speed_Delta = ((Limit_S / fabs(sRoot.Accel_Time)));
}

void SPEED_SET(double Root_Speed, double Root_Angle_Deg, double Tip_Angle_Deg){
	sRoot.Speed_Deg = Root_Speed;
	sRoot.Accel_Time = (((180 * 2) / sRoot.Speed_Deg) * 1000.) ;
	sRoot.Speed_Rps = sRoot.Speed_Deg / 360.;
	sRoot.Last_Angle_Deg = Root_Angle_Deg;
	sRoot.Last_Angle_R = sRoot.Last_Angle_Deg / 360.;

	sTip.Accel_Time = (((180 * 2) / sRoot.Speed_Deg) * 1000.);
	sTip.Speed_Deg = ((360 * 2) / (sTip.Accel_Time / 1000.));
	sTip.Speed_Rps = sTip.Speed_Deg / 360.;
	sTip.Last_Angle_Deg = Tip_Angle_Deg;
	sTip.Last_Angle_R = sTip.Last_Angle_Deg / 360.;

	PARM_SET(&pRoot,sRoot.Speed_Rps,sRoot.Last_Angle_R);
	PARM_SET(&pTip,sTip.Speed_Rps,sTip.Last_Angle_R);
}

void FIRE_FLAG_M(){
	ROOT_D = ACT;
	TIP_D = ACT;
	PS_MODE = SPEED;///
	Fire = ACT;
	//F_Button = ACT;
};

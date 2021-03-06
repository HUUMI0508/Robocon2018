/*
 * User_mura.c
 *
 *  Created on: 2018/03/30
 *      Author: RoboPro2015
 */
#include "User_mura.h"
#include "GANMO.h"

void InitUSER(void){

	InitMCTRL(&hmtr1, &henc3, 24); // Root
	InitMCTRL(&hmtr2, &henc4, 24); // Tip
	InitPI(&hmtr1.pPIParm, 0, 0, 24, -24, 1);
	InitPI(&hmtr2.pPIParm, 0, 0, 24, -24, 1);
	setPIGainCalc(&hmtr1, T_CONSTRPV_ACH, RPV_ACH, OMEGA_ACH, ZETA_ACH);
	setPIGainCalc(&hmtr2, T_CONSTRPV_BCH, RPV_BCH, OMEGA_BCH, ZETA_BCH);

	LINEAR_MOVE = NACT;
	CONTROLL_START = NACT;

	INIT_FINISH = NACT;
	//CATCH_POSITION_FINISH = NACT;
	throw_zone = Z1;
	ARM_INIT = NACT;
	CATCH = NACT;
	READY = NACT;
	THROW = NACT;
	RELOAD = NACT;
	paisen = START;
	JACK = NACT;

	MANZI_INIT();
	POSITION_INIT();
	LINEAR_INIT();
	ROTATION_INIT();

	//HAL_Delay(500);
	CONTROLL_START = ACT;
	origin = MOVE_ROOT;
	paisen = INIT;
	printf("Initialize finished\r\n");
}

void ms1(void){
	if(CONTROLL_START == ACT){
		MOTOR_CONTROLL();
		POSITION_CONTROLL();
		LINEAR_ORBIT();
		ROTATION_ORBIT();
		CATCH_SEQUENCE();
		SET_STARTING_POINT();
		PAISEN_SEQUENCE();
		SPEED_DOWN();
	}
}

void ms50(void){

	INPUT();
	if(JACK == ACT){
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else if(JACK == NACT){
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
#ifndef PRESENT

		if((paisen == FIRE) | (paisen == END)){
			EDFSetOut(0.5);
		}
		else EDFSetOut(0.0);

#endif
	}
	static int count = 0;
	count++;
	if(count >= 6){
#if 0

		printf("data start\n\r");
		if(PS_MODE == POSITION)printf("POSITION\r\n");
		if(PS_MODE == SPEED)printf("SPEED\r\n");
		if(PS_MODE == STOP)printf("STOP\r\n");
		printf("%d\r\n",origin);
		printf("%d\r\n",c_sequence);
		printf("%d\r\n",paisen);
		printf("%d\r\n",READY);
		xprint(&huart4," %d.%d %d.%d\r\n",gan((hmtr1.qDuty)),dec((hmtr1.qDuty),100),gan((hmtr2.qDuty)),dec((hmtr2.qDuty),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Speed * 360 )),dec((pRoot.Speed * 360),100),gan((pTip.Speed * 360)),dec((pTip.Speed * 360),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Angle * 360 )),dec((pRoot.Angle * 360),100),gan((pTip.Angle * 360)),dec((pTip.Angle * 360),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((henc3.qDeg)),dec((henc3.qDeg),100),gan((henc4.qDeg)),dec((henc4.qDeg),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.Inref_P)),dec((con1.Inref_P),100),gan((con2.Inref_P)),dec((con2.Inref_P),100));
		//xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.Inref)),dec((con1.Inref),100),gan((con2.Inref)),dec((con2.Inref),100));
		//xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.P_Gain)),dec((con1.P_Gain),100),gan((con2.P_Gain)),dec((con2.P_Gain),100));
		//xprint(&huart4," %d.%d \r\n",gan(protect.Lag_Count),dec(protect.Lag_Count,100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Now_Angle)),dec((pRoot.Now_Angle),100),gan((pTip.Now_Angle)),dec((pTip.Now_Angle),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan(rotation.Now_Root_Angle),dec(rotation.Now_Root_Angle,100),gan(rotation.Now_Tip_Angle),dec(rotation.Now_Tip_Angle,100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((offset_root.Angle_Deg)),dec((offset_root.Angle_Deg),100),gan((offset_tip.Angle_Deg)),dec((offset_tip.Angle_Deg),100));

#elif 1
		printf("data start\n\r");
				xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.Inref_P)),dec((con1.Inref_P),100),gan((con2.Inref_P)),dec((con2.Inref_P),100));
				xprint(&huart4," %d.%d %d.%d\r\n",gan((henc3.qDeg)),dec((henc3.qDeg),100),gan((henc4.qDeg)),dec((henc4.qDeg),100));
				xprint(&huart4," %d.%d %d.%d\r\n",gan((henc3.qDeg_m)),dec((henc3.qDeg_m),100),gan((henc4.qDeg_m)),dec((henc4.qDeg_m),100));
				xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Angle * 360 )),dec((pRoot.Angle * 360),100),gan((pTip.Angle * 360)),dec((pTip.Angle * 360),100));

#endif
		//xprint(&huart4," %d.%d %d.%d %d.%d\r\n",gan((henc1.qDeg)),dec((henc1.qDeg),100),gan((henc2.qDeg)),dec((henc2.qDeg),100),gan((henc3.qDeg)),dec((henc3.qDeg),100));
		//qENCCount
		//printf("%d	%d",henc1.qENCCount, henc2.qENCCount);
		count = 0;
	}
	return;
}

void PAISEN_SEQUENCE(){
	switch(paisen){
	case START:
		break;
	case INIT://イニシャライズ
		if(INIT_FINISH == ACT)	paisen = CATCH_POSITION;
		break;
	case CATCH_POSITION:
		INIT_FINISH = NACT;
		if(ARM_INIT == ACT){
			if(RELOAD == NACT){
				command.R1 = ACT;//受け取り位置へ
				paisen = CATCH_SHUTLE;
			}
		}
		break;
	case CATCH_SHUTLE:
		if(CATCH == ACT){
			command.SELECT = ACT;
			paisen = THROW_READY;
		}
		break;
	case THROW_READY:
		CATCH = NACT;
		if(READY == ACT){
			command.R2 = ACT;
			JACK = ACT;
			paisen = FIRE;
		}
		break;
	case FIRE:
		if(READY == NACT){
			if(THROW == ACT){
				//JACK = NACT;
				switch(throw_zone){
				case Z1:
					command.TRIANGLE = ACT;
					break;
				case Z2:
					command.CIRCLE = ACT;
					break;
				case Z3:
					command.R3 = ACT;
					break;
				}
				paisen = END;
			}
		}
		break;
	case END:
		if(THROW == NACT){
			if(LAG == NACT){
				//EDFSetOut(0.0);
				//RELOAD = ACT;//act
				//command.SQUARE = ACT;
				RELOAD = NACT;//act
				paisen = CATCH_POSITION;
			//	origin = MOVE_ROOT;
			//	paisen = INIT;
			}
		}
		break;
	}
}



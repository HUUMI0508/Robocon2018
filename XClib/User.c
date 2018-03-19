#include "User.h"
#include "GANMO.h"

void task_1(void) {

}

void InitUSER(void){

	InitMCTRL(&hmtr1, &henc1, 24); // Root
	InitMCTRL(&hmtr2, &henc2, 24); // Tip
	InitPI(&hmtr1.pPIParm, 0, 0, 24, -24, 1);
	InitPI(&hmtr2.pPIParm, 0, 0, 24, -24, 1);
	setPIGainCalc(&hmtr1, T_CONSTRPV_ACH, RPV_ACH, OMEGA_ACH, ZETA_ACH);
	setPIGainCalc(&hmtr2, T_CONSTRPV_BCH, RPV_BCH, OMEGA_BCH, ZETA_BCH);

	LINEAR_MOVE = NACT;
	CONTROLL_START = NACT;
	MANZI_INIT();
	POSITION_INIT();
	LINEAR_INIT();

	//HAL_Delay(500);
	CONTROLL_START = ACT;
	printf("Initialize finished\r\n");
}

void ms1(void){
	if(CONTROLL_START == ACT){
		MOTOR_CONTROLL();
		POSITION_CONTROLL();
		LINEAR_ORBIT();
	}
}

void ms50(void){

	INPUT();
	static int count = 0;
	count++;
	if(count >= 6){
#if 0

		printf("data start\n\r");
		if(PS_MODE == POSITION)printf("POSITION\r\n");
		if(PS_MODE == SPEED)printf("SPEED\r\n");
		if(PS_MODE == STOP)printf("STOP\r\n");
		xprint(&huart4," %d.%d %d.%d\r\n",gan((hmtr1.qDuty)),dec((hmtr1.qDuty),100),gan((hmtr2.qDuty)),dec((hmtr2.qDuty),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Speed * 360 )),dec((pRoot.Speed * 360),100),gan((pTip.Speed * 360)),dec((pTip.Speed * 360),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((pRoot.Angle * 360 )),dec((pRoot.Angle * 360),100),gan((pTip.Angle * 360)),dec((pTip.Angle * 360),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((henc1.qDeg)),dec((henc1.qDeg),100),gan((henc2.qDeg)),dec((henc2.qDeg),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.Inref_P)),dec((con1.Inref_P),100),gan((con2.Inref_P)),dec((con2.Inref_P),100));
		//xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.Inref)),dec((con1.Inref),100),gan((con2.Inref)),dec((con2.Inref),100));
		xprint(&huart4," %d.%d %d.%d\r\n",gan((con1.P_Gain)),dec((con1.P_Gain),100),gan((con2.P_Gain)),dec((con2.P_Gain),100));
		xprint(&huart4," %d.%d \r\n",gan(protect.Lag_Count),dec(protect.Lag_Count,100));
#endif
		xprint(&huart4," %d.%d %d.%d %d.%d\r\n",gan((henc1.qDeg)),dec((henc1.qDeg),100),gan((henc2.qDeg)),dec((henc2.qDeg),100),gan((henc3.qDeg)),dec((henc3.qDeg),100));
		//qENCCount
		//printf("%d	%d",henc1.qENCCount, henc2.qENCCount);
		count = 0;
	}
	return;
}


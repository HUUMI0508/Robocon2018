#include "GANMO.h"
#include "xuart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	static int counter = 1;
	if (htim->Instance == TIM13) {	//1ms
		lrf_timer ++;
		if(general_timer_flg==ACT)general_timer++;
		else general_timer=0;
#if 1/*** Encoder Calculate ***/
		CalcENC(&henc1);
		CalcENC(&henc2);

#ifndef WHEEL_ONLY
		CalcENC(&henc3);
		CalcENC(&henc4);
#endif

#endif
#if 1/*** Motor Control Calculate ***/

#ifndef WHEEL_ONLY
		ms1();
#endif

#endif
#if 1/*** Odometry Calculate ***/
	//CalcMachinePosition(&hodometry);
	if(counter >= getOdometryCycTime(&odometry_system)){
		updateMachinePosition(&odometry_system, henc1.qDeg, henc2.qDeg, hgyro.qDeg);
		//else lrf_x = 0;
		counter = 1;
	}
	else counter ++;
	if(cr_timer_flg == ACT)ms_timer ++;
	else ms_timer = 0;
#endif
		return;
	}
	if (htim->Instance == TIM11) {	//50ms
		return;
	}
	if (htim->Instance == TIM14) {	//50ms
		/*** Beep***/
		BeepEx();
#ifndef WHEEL_ONLY
		ms50();
#endif

		return;
	}
}

void InitGANMO(void) {
#if 0
	float32_t sqrt_in[2] = {2.0, 2.0};
	float32_t sqrt_out[2] = {0, 0};
	float32_t x;
	sqrt_out[0] = arm_cos_f32(sqrt_in[0]);
	arm_sqrt_f32(2.0, &x);
	sqrt_out[1] = sqrtf(x);
#endif
	setbuf(stdout, NULL);			// printf initialize
	printf("System Start...\r\n");	// printf check
	HAL_TIM_Base_Start_IT(&htim14); // 50ms timer start and beep func. start
	BeepOS(BEEP_SYSTEM_INIT_START);		// Initialize start
	while (BeepOCF()) {
		LED_ORANGE_T();
		LED_RED_T();
		LED_GREEN_T();
		HAL_Delay(100);
	}
#if 1/*** Dual Shock Initialize ***/
	if (InitDS(&huart5) == 1) {		// dual shock connect
		Beep(BEEP4_S);// connect error
		while (1)
		;
	}
#if 0/*** Dual Shock debug ***/
	while (1) {
		HAL_Delay(200);
		printf("%x\t%f\t%f\t%f\t%f\r\n", hDS.BUTTON.ButtonData, hDS.ANALOG.LX, hDS.ANALOG.LY, hDS.ANALOG.RX, hDS.ANALOG.RY);
	}
#endif
#endif
#if 1/*** Omni Initialize ***/
	mx = 0.;								//machine position x
	my = 0.;								//machine position y
	mtheta = 0.; 						//machine degree
	vx_ref = 0.;							//reference of velocity x
	vy_ref = 0.;							//reference of velocity y
	omega_ref = 0.;						//reference of angular frequency
	px_ref = 0.;							//reference of position x
	py_ref = 0.;							//reference of position y
	theta_ref = 0.;						//reference of degree
	for(int i=0;i<10;i++){
		ff_out[i]=0;
		fb_out[i]=0;
	}
	OmniSetCtrlParamsApprox(&omni_system, 0.1, 1.0);
	OmniSetMachineParams(&omni_system, N, R_W, R_M);
	OmniSetRef(&omni_system, px_ref, py_ref, theta_ref);
#endif
#if 1/*** Gyro Sensor Initialize ***/
	InitGyro(&huart8);			// gyro initialize... There is a 1 second delay in function
	GYRO_nRST(1);// gyro enable
#endif
#if 1/*** Odometry Initialize ***/
	for(int i=0;i<2;i++){
		odometry_position[i] = 0;
		odometry_speed[i] = 0;
	}
	setOdometryAxis(&odometry_system, BIAXIAL);
	setOdometryParams(&odometry_system, D_W, ODO_X_X, ODO_X_Y, ODO_Y_X, ODO_Y_Y);
	setOdometryCycTime(&odometry_system, ODO_CYC_TIME);
	lrf_x = 0;
#endif
#if 1/*** Encoder Initialize ***/
	InitENC(&henc1, ODO_ENC_X, ENC_CYC_TIME, 1, GEAR_RATIO, E_PPR, CLOCKWISE);				// encoder initialize
	InitENC(&henc2, ODO_ENC_Y, ENC_CYC_TIME, 1, GEAR_RATIO, E_PPR, COUNTERCLOCKWISE);				// encoder initialize
	//InitENC(&henc3, ENC3, 1, 1 / 26.0, 1, 100, CLOCKWISE);				// encoder initialize
	//InitENC(&henc4, ENC4, 1, 1 / 26.0, 1, 100, CLOCKWISE);				// encoder initialize

#ifndef WHEEL_ONLY
	InitENC(&henc3, ENC3, 1, GEAR_RATIO_Gm_ACH, GEAR_RATIO_Ge_ACH, PPR_ACH, COUNTERCLOCKWISE);//Root
	InitENC(&henc4, ENC4, 1, GEAR_RATIO_Gm_BCH, GEAR_RATIO_Ge_BCH, PPR_BCH, COUNTERCLOCKWISE);//Tip
#endif

#endif
#if 0/*** Motor Control Initialize in GANMO***/
	InitMCTRL(&hmtr1, &henc2, 0.5);
	setPGainCalc(&hmtr1, 1000);				// position control gain
	setPIGainCalc(&hmtr1, 35, 317, 20, 1);// velocity control gain
#endif
#if 1
	/*** CAN Initialize ***/
	InitCAN(&hcan1);				// can1 initialize
	InitMotorDriver(&hcan1, &omni_motor[2], ID7, BCH, MODE_ClosedLoop);
	MotorDriveClosedLoop(&omni_motor[0], 0);
	InitMotorDriver(&hcan1, &omni_motor[1], ID8, ACH, MODE_ClosedLoop);
	MotorDriveClosedLoop(&omni_motor[1], 0);
	InitMotorDriver(&hcan1, &omni_motor[0], ID9, ACH, MODE_ClosedLoop);
	MotorDriveClosedLoop(&omni_motor[2], 0);
	InitMotorDriver(&hcan1, &omni_motor[3], ID9, BCH, MODE_ClosedLoop);
	MotorDriveClosedLoop(&omni_motor[3], 0);

#ifndef WHEEL_ONLY
	InitMotorDriver(&hcan1, &md1, ID1, ACH, MODE_OpenLoop);
	MotorDriveOpenLoop(&md1, 0);
	InitMotorDriver(&hcan1, &md2, ID1, BCH, MODE_OpenLoop);
	MotorDriveOpenLoop(&md2, 0);// can1 initialize
#endif

	StartCAN(&hcan1);
#endif
	HAL_Delay(500);
	/*** Initialize finished***/
	BeepOS(BEEP_SYSTEM_INIT_FINISH);		// Initialize finished
	uint8_t led_pattern = 0;
	while (BeepOCF()) {
		if (++led_pattern == 1) {
			LED_ORANGE_HIGH();
			LED_GREEN_LOW();
		} else if (led_pattern == 2) {
			LED_ORANGE_LOW();
			LED_RED_HIGH();
		} else if (led_pattern == 3) {
			led_pattern = 0;
			LED_RED_LOW();
			LED_GREEN_HIGH();
		}
		HAL_Delay(50);
	}
	LED_RED_LOW();
	LED_GREEN_LOW();
	LED_ORANGE_LOW();
	//while (!hDS.BUTTON.START);
	MCPS_HIGH(); // main circuit power supply enable
	/*** 1ms timer Initialize ***/
	lrf_timer = 0;
	general_timer_flg=NACT;
	general_timer=0;

	HAL_TIM_Base_Start_IT(&htim11); // 50ms timer start and beep func. start
	HAL_TIM_Base_Start_IT(&htim13); // 1ms timer start and start measurement of encoder
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan->Instance == CAN1) {
		TransmitCAN(hcan);
		return;
	}
	if (hcan->Instance == CAN2) {
		return;
	}
	if (hcan->Instance == CAN3) {
		return;
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
#if 0
#define   HAL_CAN_ERROR_NONE      0x00000000U    /*!< No error             */
#define   HAL_CAN_ERROR_EWG       0x00000001U    /*!< EWG error            */
#define   HAL_CAN_ERROR_EPV       0x00000002U    /*!< EPV error            */
#define   HAL_CAN_ERROR_BOF       0x00000004U    /*!< BOF error            */
#define   HAL_CAN_ERROR_STF       0x00000008U    /*!< Stuff error          */
#define   HAL_CAN_ERROR_FOR       0x00000010U    /*!< Form error           */
#define   HAL_CAN_ERROR_ACK       0x00000020U    /*!< Acknowledgment error */
#define   HAL_CAN_ERROR_BR        0x00000040U    /*!< Bit recessive        */
#define   HAL_CAN_ERROR_BD        0x00000080U    /*!< LEC dominant         */
#define   HAL_CAN_ERROR_CRC       0x00000100U    /*!< LEC transfer error   */
#define   HAL_CAN_ERROR_FOV0      0x00000200U    /*!< FIFO0 overrun error  */
#define   HAL_CAN_ERROR_FOV1      0x00000400U    /*!< FIFO1 overrun error  */
#define   HAL_CAN_ERROR_TXFAIL    0x00000800U    /*!< Transmit failure     */
#endif
	if (hcan->Instance == CAN1) {
		return;
	}
	if (hcan->Instance == CAN2) {
		return;
	}
	if (hcan->Instance == CAN3) {
		return;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#if 1/*** Gyro Calculate ***/
	if (huart->Instance == UART8) {
		CalcGyro();
		return;
	}
#endif
#if 1/*** DualShock Calculate ***/
	if (huart->Instance == UART5) {
		getDSdata();
		return;
	}
#endif
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	/*** DualShock ***/
	if (huart->Instance == UART5) {
		if (huart->ErrorCode & HAL_UART_ERROR_ORE) {
			huart->Instance->ICR = USART_ICR_ORECF;
			printf("ORE\r\n");
		}
		if (huart->ErrorCode & HAL_UART_ERROR_DMA) {
			printf("DMA\r\n");
		}
		if (huart->ErrorCode & HAL_UART_ERROR_FE) {
			huart->Instance->ICR = USART_ICR_FECF;
			printf("FE\r\n");
		}
		if (huart->ErrorCode & HAL_UART_ERROR_NE) {
			huart->Instance->ICR = HAL_UART_ERROR_NE;
			printf("NE\r\n");
		}
		if (huart->ErrorCode & HAL_UART_ERROR_PE) {
			huart->Instance->ICR = USART_ICR_PECF;
			printf("PE\r\n");
		}
		huart->Instance->RDR;
		ReStartDS();
		return;
	}
	/*** Gyro ***/
	if (huart->Instance == UART8) {
		if (huart->ErrorCode == HAL_UART_ERROR_ORE) {
			huart->Instance->ISR &= ~0x0020;
		}
		huart->Instance->RDR;
		return;
	}
#if 0
	if (huart->Instance == UART4) {
		if (huart->ErrorCode == HAL_UART_ERROR_ORE) {
			huart->Instance->ISR &= ~0x0020;
		}
		huart->Instance->RDR;
		HAL_UART_Receive_IT(huart, &buf, 1);
		return;
	}
	if (huart->Instance == UART8) {
	}
#endif
}

#if 0
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
//		isr_tx(huart);
	}
}
#endif

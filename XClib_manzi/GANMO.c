#include "GANMO.h"
#include "xuart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM13) {	//1ms
#if 0/*** Odometry Calculate ***/
	CalcMachinePosition(&hodometry);
#endif
#if 1/*** Encoder Calculate ***/
		CalcENC(&henc1);
		CalcENC(&henc2);
		CalcENC(&henc3);
		//CalcENC(&henc4);
#endif
#if 1/*** Motor Control Calculate ***/
		ms1();
#endif
		return;
	}
	if (htim->Instance == TIM14) {	//50ms
#if 1/*** Beep***/
		BeepEx();
#endif
//		printf("%x\r\n", hDS.BUTTON.ButtonData);
		ms50();
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
	while (BeepOCF() > 0) {
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
#if 0/*** Omni Initialize ***/
	InitOmni(OMNI_4_F, 0.55, 0.1);				// omni initialize
#endif
#if 0/*** Gyro Sensor Initialize ***/
	InitGyro(&huart8);			// gyro initialize... There is a 1 second delay in function
	GYRO_nRST(1);// gyro enable
#endif
#if 0/*** Odometry Initialize ***/
	InitOdometry (&hodometry, &henc1, &henc2, &hgyro, 1, 0.048);
#endif
#if 1/*** Encoder Initialize ***/
	InitENC(&henc1, ENC1, 1, GEAR_RATIO_Gm_ACH, GEAR_RATIO_Ge_ACH, PPR_ACH, COUNTERCLOCKWISE);//Root
//	InitENC(&henc1, ENC1, 1, 1/1, 1, 100, CLOCKWISE);//Root
	InitENC(&henc2, ENC2, 1, GEAR_RATIO_Gm_BCH, GEAR_RATIO_Ge_BCH, PPR_BCH, COUNTERCLOCKWISE);//Tip
	InitENC(&henc3, ENC3, 1, 14 / 46.0, 1, 100, CLOCKWISE);				// encoder initialize
//	InitENC(&henc4, ENC4, 1, 1 / 26.0, 1, 100, CLOCKWISE);				// encoder initialize
#endif
#if 0/*** Motor Control Initialize in GANMO***/
	InitMCTRL(&hmtr1, &henc2, 0.5);
	setPGainCalc(&hmtr1, 1000);				// position control gain
	setPIGainCalc(&hmtr1, 35, 317, 20, 1);// velocity control gain
#endif
#if 1
	/*** CAN Initialize ***/
	InitCAN(&hcan1);				// can1 initialize
	InitMotorDriver(&hcan1, &md1, ID1, ACH, MODE_OpenLoop);
	MotorDriveOpenLoop(&md1, 0);
	InitMotorDriver(&hcan1, &md2, ID2, BCH, MODE_OpenLoop);
	MotorDriveOpenLoop(&md2, 0);// can1 initialize
	StartCAN(&hcan1);
#endif
	HAL_Delay(500);
	/*** Initialize finished ***/
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
	InitUSER();
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
#if 0/*** Gyro Calculate ***/
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
	uint8_t buf = 0;
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
#if 0
	/*** Gyro ***/
	if (huart->Instance == UART8) {
		if (huart->ErrorCode == HAL_UART_ERROR_ORE) {
			huart->Instance->ISR &= ~0x0020;
		}
		huart->Instance->RDR;
		return;
	}
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

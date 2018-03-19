#ifndef DUALSHOCK_H_
#define DUALSHOCK_H_

#include "../Inc/usart.h"

#define BACKLASH 28

typedef struct st_ds {
	union {
		uint16_t ButtonData;
		struct {
			union {
				uint8_t ButtonLowData	:8;
				struct {
					uint8_t L2			:1;
					uint8_t R2			:1;
					uint8_t L1			:1;
					uint8_t R1			:1;
					uint8_t TRIANGLE	:1;
					uint8_t CIRCLE		:1;
					uint8_t CROSS		:1;
					uint8_t SQUARE		:1;
				};
			};
			union {
				uint8_t ButtonHighData	:8;
				struct {
					uint8_t SELECT		:1;
					uint8_t L3			:1;
					uint8_t R3			:1;
					uint8_t START		:1;
					uint8_t UP			:1;
					uint8_t RIGHT		:1;
					uint8_t DOWN		:1;
					uint8_t LEFT		:1;
				};
			};
		};
	}BUTTON;
	struct {
		float LY;
		float LX;
		float RY;
		float RX;
	}ANALOG;
} tDSParm;

extern tDSParm hDS;

/**
 * @brief Function to initialize the encoder
 * @param mode		see definition above
 */
extern uint8_t InitDS(UART_HandleTypeDef *huart);
extern void ReStartDS(void);
/**
 * @brief Function to initialize the encoder
 * @retval *pParm	see definition above
 */
extern void getDSdata(void);
#endif /* DUALSHOCK_H_ */

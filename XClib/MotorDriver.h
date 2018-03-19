#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_
#include "../Inc/can.h"

typedef enum {
	ACH = 0, BCH = 0x80
} tMTR_CHANNEL;

/**
 * @brief mode select
 */
typedef enum {
	MODE_NOT_USE_CH = 0, MODE_OpenLoop, MODE_ClosedLoop, MODE_Servo
//	MODE_Current
} tMTR_MODE;
#define MODE_MASK 0x0f

/**
 * @brief ID select
 */
typedef enum {
	ID0 = 0, ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, ID9, IDA, IDB, IDC, IDD, IDE, IDF
} tMTR_ID;

/**
 * @brief buf
 */
#define MAX_DUTY	1000
#define MAX_RPS		500.0

typedef struct {
	CanTxMsgTypeDef TxMessage;
	uint8_t qCANcycleNum;
} tMD;

typedef union IntCnvFloat {
	uint32_t ival;
	float fval;
} tICF;

/**
 * @brief Init
 * @param *hParm
 * @param id
 * @param mode
 * @param ch
 */
extern void InitMotorDriver(CAN_HandleTypeDef* hcan, tMD *pParm, tMTR_ID id, tMTR_CHANNEL ch, tMTR_MODE mode);

extern void MotorDriveOpenLoop(tMD *pParm, int16_t pwm);

/**
 * @brief duty ratio or Speed command value
 * @param *hParm
 * @param control
 */
extern void MotorDriveClosedLoop(tMD *pParm, float rps);

extern void MotorDriveServo(tMD *pParm, tICF angle, float max_rps);

#endif /* MOTORDRIVER_H_ */

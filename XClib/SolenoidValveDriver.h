#ifndef SOLENOIDVALVEDRIVER_H_
#define SOLENOIDVALVEDRIVER_H_
#include "../Inc/can.h"

typedef enum {
	NO1_HIGH = 0x0001,
	NO2_HIGH = 0x0002,
	NO3_HIGH = 0x0004,
	NO4_HIGH = 0x0008,
	NO5_HIGH = 0x0010,
	NO6_HIGH = 0x0020,
	NO7_HIGH = 0x0040,
	NO8_HIGH = 0x0080,
	NO1_LOW  = 0x0100,
	NO2_LOW  = 0x0200,
	NO3_LOW  = 0x0400,
	NO4_LOW  = 0x0800,
	NO5_LOW  = 0x1000,
	NO6_LOW  = 0x2000,
	NO7_LOW  = 0x4000,
	NO8_LOW  = 0x8000
} tSVDType;

typedef struct {
	CanTxMsgTypeDef TxMessage;
} tSVDParm;

//extern tSVDParm hsvd;

#endif /* SOLENOIDVALVEDRIVER_H_ */

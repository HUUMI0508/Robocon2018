#include "SolenoidValveDriver.h"

void InitSolenoidValveDriver (tSVDParm *pParm, uint32_t id) {
	pParm->TxMessage.StdId = id;
	pParm->TxMessage.RTR = CAN_RTR_DATA;
	pParm->TxMessage.IDE = CAN_ID_STD;
	pParm->TxMessage.DLC = 2;
	pParm->TxMessage.Data[0] = 0x00;
	pParm->TxMessage.Data[1] = 0xff;
}

void SolenoidValveDrive (tSVDParm *pParm, tSVDType data) {
	pParm->TxMessage.Data[0] = (uint8_t)(data & 0xff);
	pParm->TxMessage.Data[1] = (uint8_t)((data >> 8) & 0xff);
}

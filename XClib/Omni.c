#include "math.h"
#include "Omni.h"

static float omni_coefficient[8];
static uint8_t wheel_sum;
static float R_r_ratio;

void InitOmni(tOMNITYPE type, float WheelFromCenterOfMachine_meter, float Wheel_meter) {
	float R, r;
	R = WheelFromCenterOfMachine_meter;
	r = Wheel_meter;
	R_r_ratio = R / r;
	switch (type) {
	case OMNI_4_F:
		omni_coefficient[0] = -1.0 / sqrt(2.0);
		omni_coefficient[1] = 1.0 / sqrt(2.0);
		omni_coefficient[2] = -1.0 / sqrt(2.0);
		omni_coefficient[3] = -1.0 / sqrt(2.0);
		omni_coefficient[4] = 1.0 / sqrt(2.0);
		omni_coefficient[5] = -1.0 / sqrt(2.0);
		omni_coefficient[6] = 1.0 / sqrt(2.0);
		omni_coefficient[7] = 1.0 / sqrt(2.0);
		break;
	case OMNI_4_P:
		omni_coefficient[0] = 0.0;
		omni_coefficient[1] = 1.0;
		omni_coefficient[2] = -1.0;
		omni_coefficient[3] = 0.0;
		omni_coefficient[4] = 0.0;
		omni_coefficient[5] = -1.0;
		omni_coefficient[6] = 1.0;
		omni_coefficient[7] = 0.0;
		break;
	case OMNI_3_F:
		omni_coefficient[0] = -1.0 / 2.0;
		omni_coefficient[1] = sqrt(3.0) / 2.0;
		omni_coefficient[2] = -1.0 / 2.0;
		omni_coefficient[3] = -sqrt(3.0) / 2.0;
		omni_coefficient[4] = 1.0;
		omni_coefficient[5] = 0.0;
		omni_coefficient[6] = 0.0;
		omni_coefficient[7] = 0.0;
		break;
	case OMNI_3_P:
		omni_coefficient[0] = -1.0;
		omni_coefficient[1] = 0.0;
		omni_coefficient[2] = 1.0 / 2.0;
		omni_coefficient[3] = -sqrt(3.0) / 2.0;
		omni_coefficient[4] = 1.0 / 2.0;
		omni_coefficient[5] = sqrt(3.0) / 2.0;
		omni_coefficient[6] = 0.0;
		omni_coefficient[7] = 0.0;
		break;
	}
	for (uint8_t i = 0; i < 8; i++) {
		omni_coefficient[i] /= (2.0 * M_PI * R);
	}
	if (type < 0) {
		wheel_sum = -type;
	} else {
		wheel_sum = type;
	}
}
// omega [rad/s]
// v = r * omega = r * 2 * pi * rps
// rps = v / (2*pi*r)
void CalcOmni(float freq[], float vx, float vy, float omega_rps, float theta) {
	float _cos = cos(theta);
	float _sin = sin(theta);
	float vx_m = vx * _cos + vy * _sin;
	float vy_m = vx * -_sin + vy * _cos;

	for (uint8_t i = 0; i < wheel_sum; i++) {
		freq[i] = R_r_ratio * (omni_coefficient[2 * i] * vx_m + omni_coefficient[2 * i + 1] * vy_m + omega_rps);
	}
}

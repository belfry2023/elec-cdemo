#ifndef GENERAL_DEF_H
#define GENERAL_DEF_H

#include "stdint.h"

#define NOW 0
#define LAST 1
// 一些module的通用数值型定义,注意条件macro兼容,一些宏可能在math.h中已经定义过了

#ifndef PI
#define PI 3.1415926535f
#endif
#define PI2 (PI * 2.0f) // 2 pi

#define RAD_2_DEGREE 57.2957795f    // 180/pi
#define DEGREE_2_RAD 0.01745329252f // pi/180

#define RPM_2_ANGLE_PER_SEC 6.0f       // ×360°/60sec
#define RPM_2_RAD_PER_SEC 0.104719755f // ×2pi/60sec

uint16_t float_to_uint(float x, float x_min, float x_max, uint8_t bits);

float uint_to_float(int x_int, float x_min, float x_max, int bits);


#endif // !GENERAL_DEF_H
#ifndef _MYMATH_H_
#define _MYMATH_H_
#include "headfile.h"


float Smooth_filter(float *value_buf,uint8_t size,float NewValue);
int16_t Smooth_filter_int(int16_t *value_buf,uint8_t size,int16_t NewValue);
int16_t Weighted_average_filter(int16_t *value_buf,uint8_t size,int16_t NewValue);
int16_t Limiting(int16_t duty, int16_t min, int16_t max);
uint16_t Limiting_uint16(uint16_t duty, uint16_t min, uint16_t max);
float Limiting_float(float duty, float min, float max);
int16_t My_abs(int16_t Value);
float firstOrderLowPassFilter(float parameter,float *input,float *Last_output);
float InvSqrt(float x);

#endif



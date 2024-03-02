#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "headfile.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //103的定时器是16位 2的16次方最大是65536

void A_Encoder_Init(void);
void B_Encoder_Init(void);

void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);

int16_t Read_Encoder(u8 TIMX);
void getSpeed(int16_t *leftEncoder,int16_t *rightEncoder);
void Encoder_TIM_Test(void);

#endif



#ifndef _EXIT_H_
#define _EXIT_H_
#include "headfile.h"


extern u8 task_direct,task_speed,task_stability,task_oled,task_rgb;


void MPU6050_EXTI_Init(void);
void EXTI9_5_IRQHandler(void);

#endif



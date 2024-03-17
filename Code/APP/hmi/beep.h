#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x_gpio.h"

#define BEEP_OFF GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define BEEP_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_12)

void Beep_Init(void);


#endif



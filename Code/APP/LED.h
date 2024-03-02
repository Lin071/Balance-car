#ifndef _LED_H_
#define _LED_H_
#include "headfile.h"

#define LED1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LED0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)


#define BEEP_OFF GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define BEEP_ON GPIO_ResetBits(GPIOA,GPIO_Pin_12)


void LED_Init(void);
void Write0(void);
void Write1(void);
void Reset(void);
void LED_Set_Byte(uint8_t data);
void RGB_LED_Colour_Set(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Rand(void);
void RGB_LED_Runing(void);
void RGB_LED_Red(void);
void RGB_LED_Orange(void);
void RGB_LED_Yellow(void);
void RGB_LED_green(void);
void RGB_LED_Cyan(void);
void RGB_LED_Blue(void);
void RGB_LED_Violet(void);
void RGB_LED_FLY(void);
void RGB_LED_White(void);
void RGB_LED_Off(void);


#endif



#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include "stm32f10x_gpio.h"


#define RGB_OFF      0 
#define RGB_RUNNING  1
#define RGB_FLY      2
#define RGB_RED      3
#define RGB_BLUE     4
#define RGB_YELLOW   5
#define RGB_WHITE    6



#define LED1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LED0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)


#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_13)



void Led_Init(void);
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



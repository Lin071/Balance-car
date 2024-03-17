#include "beep.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


/**************************************************************************
*功  能：蜂鸣器GPIO初始化
*参  数：无 
*返回值：无
*备  注：初始化PA12
**************************************************************************/
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟

	//蜂鸣器初始化
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;				//配置为PA12端口(蜂鸣器)
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);	
	BEEP_OFF;
}



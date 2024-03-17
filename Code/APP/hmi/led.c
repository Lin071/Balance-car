#include "led.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//跑马灯RGB三元色配出七彩跑马灯
static uint32_t Run_buf[][16] = 
{
	{0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,},//橙色
	{0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,},//绿色
	{0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,},//紫色
	{0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,},//青色
	{0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,},//蓝色
	{0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,},//黄色
	{0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,},//白色
};

/**************************************************************************
*功  能：LED接口初始化
*参  数：无 
*返回值：无
*备  注：初始化PB9(RGB灯)
**************************************************************************/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); //使能端口时钟

	//RGB灯初始化
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;					//配置为PB8端口(RGB灯)
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);						//初始化PB1端口
	RGB_LED_Off();

	//核心板绿色LED灯
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;					//配置为PC13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	LED_OFF;
}



/***************************************************************************
*函  数：void Write0(void)
*功  能：写0
*参  数：无
*返回值：无
*备  注: 不同的系统时钟频率下需要微调
***************************************************************************/
void Write0(void)
{
	uint8_t cnt2 = 4;
	LED1;
//	while(cnt1--)  //高电平300ns
	__nop();        

    LED0;
	while(cnt2--)  //低电平900ns
		__nop();
}

/***************************************************************************
*函  数：void Write1(void)
*功  能：写1
*参  数：无
*返回值：无
*备  注: 不同的系统时钟频率下需要微调
***************************************************************************/
void Write1(void)
{
	uint8_t cnt1=3,cnt2 = 3;
	LED1;
	while(cnt1--) //高电平600ns
		__nop();

  	LED0;
	while(cnt2--) //低电平600ns
		__nop();

}

/***************************************************************************
*函  数：void Reset(void)
*功  能：RGB灯复位
*参  数：无
*返回值：无
*备  注: 无
***************************************************************************/
void Reset(void)
{
	u16 cnt1 = 2500;
	LED0;
	while(cnt1--)
		__nop();
  LED1;
}

/***************************************************************************
*函  数：Lvoid LED_Set_Byte(uint8_t data)
*功  能：写一个字节数据
*参  数：data 
*返回值: 无
*备  注: 无
***************************************************************************/
void LED_Set_Byte(uint8_t data)
{
	int i=0;
	for(i=0;i<8;i++)
	{
	  	if((data<<i)&0x08)
				Write1();
			else
				Write0();
	}
}

/***************************************************************************
*函  数：void RGB_LED_Colour_Set(uint8_t green,uint8_t red,uint8_t blue)
*功  能：设置一个灯的色彩
*参  数：green red blue，红绿蓝光所占比例大小,范围0~255
*返回值: 无
*备  注: 无
***************************************************************************/
void RGB_LED_Colour_Set(uint8_t green,uint8_t red,uint8_t blue)
{
	LED_Set_Byte(green);
	LED_Set_Byte(red);
	LED_Set_Byte(blue);
}

/**************************************************************************
*函  数：void RGB_LED_Runing(void)
*功  能：随机变换颜色
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Rand(void)
{
	uint8_t i,red=0,green=0,blue=0;;
	for(i=0;i<4;i++)
	{
		green = rand()%18+2;//产生一个0~20的随机数
		red   = rand()%18+2;
		blue  = rand()%18+2;
    	RGB_LED_Colour_Set(green,red,blue);//合成颜色
	}
 // Reset();        //复位显示 
//	delay_ms(300);	  
}

/**************************************************************************
*函  数：void RGB_LED_Runing(void)
*功  能：跑马灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Runing(void)
{
	uint8_t i,red=0,green=0,blue=0;
	static uint8_t cnt = 0,wcnt = 0,times = 0;
	if(times++ >= 16)
	{
		times = 0; 
		wcnt++;
	}
	for(i=0;i<4;i++)
	{
		if(cnt>4) cnt = 0;
		red   = ((Run_buf[wcnt][cnt]>>16)&0xff);
		green = ((Run_buf[wcnt][cnt]>>8)&0xff); 
		blue  = ((Run_buf[wcnt][cnt]>>0)&0xff);
		RGB_LED_Colour_Set(green,red,blue);//合成颜色
		cnt++;
	}
	if(wcnt==7) wcnt = 0;
 // Reset();        //复位显示 
 //	delay_ms(200);	  
}

/**************************************************************************
*函  数：void RGB_LED_Red(void)
*功  能：红灯
*参  数：无
*返回值：无
*备  注：还有很多种颜色可以自己慢慢调配
**************************************************************************/
void RGB_LED_Red(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0,0xFF,0);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Orange(void)
*功  能：橙灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Orange(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0xA5,0xFF,0x00);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Yellow(void)
*功  能：黄灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Yellow(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0xFF,0xFF,0);
	}
}

/**************************************************************************
*函  数：void RGB_LED_green(void)
*功  能：绿灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_green(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0xFF,0,0);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Cyan(void)
*功  能：青灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Cyan(void) 
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0xFF,0,0xFF);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Blue(void)
*功  能：蓝灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Blue(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0,0,0xFF);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Violet(void)
*功  能：紫灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Violet(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0x00,0xCD,0xCD);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Violet(void)
*功  能：两红两绿
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_FLY(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(i<2)
    		RGB_LED_Colour_Set(0xFF,0,0);
		else
			RGB_LED_Colour_Set(0,0xFF,0);
	}
}

/**************************************************************************
*函  数：void RGB_LED_White(void)
*功  能：白灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_White(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    	RGB_LED_Colour_Set(0x0F,0x0F,0x0F);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Off(void)
*功  能：关闭LED灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Off(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_LED_Colour_Set(0,0,0);
	}
}








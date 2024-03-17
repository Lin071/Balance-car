#include "encoder.h"

#include <stdio.h>
#include <stdint.h>

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "control.h"


/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式（读取左车轮编码速度）
入口参数：无
返回  值：无
**************************************************************************/
static void A_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PA端口时钟
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置(PA0\PA1)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOA
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 		//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE); 
}

/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式（读取右车轮编码速度）
入口参数：无
返回  值：无
**************************************************************************/
static void B_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置(PTB6\PTB7)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}


/**************************************************************************
函数功能：初始化小车TIM2和TIM4，用于读取小车的编码器转速
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init(void)
{
	A_Encoder_Init();	//初始化左轮编码器定时器TIM2
	B_Encoder_Init();	//初始化右轮编码器定时器TIM4
}


/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
static int16_t Read_Encoder(uint8_t TIMX)
{
	 int16_t Encoder_TIM;    
	 switch(TIMX)
	 {
		 case 2:  Encoder_TIM = (short)TIM2 -> CNT;  TIM2 -> CNT=0; break;
		 case 3:  Encoder_TIM = (short)TIM3 -> CNT;  TIM3 -> CNT=0; break;	
		 case 4:  Encoder_TIM = (short)TIM4 -> CNT;  TIM4 -> CNT=0; break;	
		 default: Encoder_TIM = 0;
	 }
	 return Encoder_TIM;
}

/**************************************************************************
函数功能：获取左右两轮的速度
入口参数：传入一个指针，用于读取速度
返回  值：空
**************************************************************************/
void getSpeed(int16_t *p_leftEncoder,int16_t *p_rightEncoder)
{
    *p_leftEncoder = Read_Encoder(2);
    *p_rightEncoder = -Read_Encoder(4);
}


/**************************************************************************
函数功能：测试TIM2\TIM4读取编码器功能函数
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_TIM_Test(void)
{
	int16_t Encoder_A = 0;
	int16_t Encoder_B = 0;

	Encoder_A = Read_Encoder(2);
	Encoder_B = -Read_Encoder(4);

	printf("d:%d,%d\n",Encoder_A,Encoder_B);
}


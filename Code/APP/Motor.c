#include "Motor.h"

/**************************************************************************
//函数功能：PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//TIM2_PWM_Init(7199,0);//PWM频率=72000/(7199+1)=10Khz
**************************************************************************/
void BMotor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能定时器3时钟使能 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIOB外设时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3预装载使能	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}

/**************************************************************************
//函数功能：PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//TIM4_PWM_Init(7199,0);//PWM频率=72000/(7199+1)=10Khz
**************************************************************************/
void AMotor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
   //设置该引脚为复用输出功能,输出TIM3 CH3 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH3  TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

    TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE 主输出使能	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH4预装载使能	 
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3

}

/**************************************************************************
函数功能：初始化输出电机方向引脚
入口参数：无
返回  值：无
**************************************************************************/
void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MHZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
    AIN1=0,AIN2=0;
    BIN1=0,BIN2=0;

    //=====初始化PWM 10KHZ,用于驱动电机
    AMotor_PWM_Init(7199,0);
    BMotor_PWM_Init(7199,0);

    //使电机停止
    setMotorPwm(0,0);

}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void setMotorPwm(int16_t motoA,int16_t motoB)
{  	
    motoA = Limiting(motoA,-7100,7100);	
    motoB = Limiting(motoB,-7100,7100);

    if(motoB < 0)	 BIN2=0,	BIN1=1;
    else 	         BIN2=1,	BIN1=0;
    TIM_SetCompare3(TIM3,My_abs(motoB));

    if(motoA < 0)	 AIN1=1,	AIN2=0;
    else        	 AIN1=0,	AIN2=1;
    TIM_SetCompare4(TIM3,My_abs(motoA));
}


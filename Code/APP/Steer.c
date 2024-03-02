#include "Steer.h"

/**************************************************************************
//函数功能：舵机PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//Steer_Pwm_Init(1999,719);//PWM频率=72000/(1999+1)/(719+1)=50hz
**************************************************************************/
void Steer_Pwm_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);  //使能GPIO外设时钟使能
	
    //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1 和 TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//<------就是这句话
    
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}

// 舵机初始化
void Steer_Init(void)
{
    //=====初始化PWM 50HZ,用于驱动舵机
    Steer_Pwm_Init(1999,719);
    
    // 使舵机复位，恢复中值位置
    setSteerPwm(FRONTSTEER_MIDIAN,REARSTEER_MIDIAN);
}

// 双舵机控制输出同步
void steerSynchronousOut(float pwmSteerOut)
{
    uint16_t tempFrontOut,tempRearOut;
    tempFrontOut = (uint16_t)(FRONTSTEER_MIDIAN - pwmSteerOut);
    tempRearOut = (uint16_t)(REARSTEER_MIDIAN + pwmSteerOut);
    
    setSteerPwm(tempFrontOut,tempRearOut);
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：舵机的占空比(2.5%~12.5%)
返回  值：无

                车前

 前舵机  230 ---  150  --- 80

          80 ---  160  --- 230   后舵机

                车尾

**************************************************************************/
void setSteerPwm(u16 pwmFront,u16 pwmRear)
{
    // 更改前面舵机的PWM
    pwmFront = Limiting_uint16(pwmFront,80,230);//中心位置150
    TIM_SetCompare1(TIM1,pwmFront);
    
    // 更改后面电机的PWM
    pwmRear = Limiting_uint16(pwmRear,80,230);  //中心位置160
    TIM_SetCompare4(TIM1,pwmRear);
}


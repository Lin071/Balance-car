#include "Exit.h"

u8 task_direct,task_speed,task_stability,task_oled,task_rgb;



/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
作    用：是用来配置MPU6050引脚INT的，每当MPU6050有数据输出时，引脚INT有相应的电平输出。
					依次来触发外部中断作为控制周期。保持MPU6050数据的实时性。(5ms进一次中断)
**************************************************************************/
void MPU6050_EXTI_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//外部中断，需要使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	            //端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化PTB5
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    //////////////////外部中断5优先级配置也就是MPU6050 INT引脚的配置///////////因为是控制中断，故此优先级应是最高。
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级0， 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
函数功能：程序的定时任务切换
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步	
				 在MPU6050的采样频率设置中，设置成100HZ，即可保证6050的数据是10ms更新一次。
				 在imv_mpu.h文件第26行的宏定义进行修改(#define DEFAULT_MPU_HZ  (100))
**************************************************************************/
void EXTI9_5_IRQHandler(void) 
{    
    static u8 task10ms,task20ms,task50ms,task100ms,task200ms;
    if(PBin(5)==0)		//判断是否MUP6050发生的中断
    {	
       EXTI->PR=1<<5;     //===清除LINE5上的中断标志位 
       task10ms++;
       task20ms++;
       task50ms++;
       task100ms++;
       task200ms++;
    
       if(task10ms >= 2){
           task10ms = 0;
           task_direct = 1;
       }
       
       if(task20ms >= 4){
           task20ms = 0;
           task_stability = 1;
       }
       
       if(task50ms >= 10){
           task50ms = 0;
           task_speed = 1;
       }
       
       if(task100ms >= 20){
           task100ms = 0;
           task_rgb = 1;
       }
       
        if(task200ms >= 40){
           task200ms = 0;
           task_oled = 1;
       }
    }
}


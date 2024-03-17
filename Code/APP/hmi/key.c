#include "key.h"

#include <stdint.h>

#include "FreeRTOS.h"
#include "event_groups.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "sys.h"


#include "control.h"


/**************************************************************************
函数功能：LED接口初始化
入口参数：无 
返回  值：无
**************************************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能端口时钟

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	//外部已上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure);			//初始化PA端口				
}



/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
作    用：是用来配置MPU6050引脚INT的，每当MPU6050有数据输出时，引脚INT有相应的电平输出
					依次来触发外部中断作为控制周期。保持MPU6050数据的实时性。(5ms进一次中断)
**************************************************************************/
void Key_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 映射PA2到EXTI Line2
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line= EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    // 映射PA3到EXTI Line3
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
    EXTI_InitStructure.EXTI_Line= EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    /////////////////////////////配置NVIC/////////////////////////////////
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;		//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;        //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;        //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

// 按键SW2外部中断处理函数
void EXTI2_IRQHandler(void) 
{

  if(PAin(2)==0)		//判断是否是PA2发生的中断(UP)
  {	
      EXTI->PR=1<<2;      //===清除LINE2上的中断标志位

      /* 发出事件: OLED页面往前翻一页 */
      xEventGroupSetBitsFromISR(xEventGroup, UP, NULL);

    // 问题：想要使用 xEventGroupSetBitsFromISR 函数，就需要在 "FreeRTOSConfig.h" 中定义以下：
    /*  #define INCLUDE_xEventGroupSetBitFromISR        1
     *  #define INCLUDE_xTimerPendFunctionCall          1
     *  #define configUSE_TIMERS                        1
     */
  }
}


// 按键SW4外部中断处理函数
void EXTI3_IRQHandler(void) 
{

  if(PAin(3)==0)		//判断是否是PA3发生的中断(DOWN)
  {	
    EXTI->PR=1<<3;      //===清除LINE3上的中断标志位
    
    /* 发出事件: OLED页面往后翻一页 */
    xEventGroupSetBitsFromISR(xEventGroup, DOWN, NULL);
  }
}


/***********************************************************************/
//		    程序说明
//此程序用于：两轮平衡小车
//所用MCU芯片：STM32F103C8T6
//作者：071
//
/***********************************************************************/

#include "stm32f10x_rcc.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#include "sys.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "motor.h"
#include "encoder.h"
#include "oled.h"
#include "remote.h"
#include "control.h"
#include "mpu6050.h"




/* 硬件初始化函数声明 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/



/***************************************************************************
*函  数：static void vInitTask(void *pvParameters)
*功  能：初始化任务，用于初始化小车模块以及创建其他模块
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vInitTask(void *pvParameters)
{
	taskENTER_CRITICAL();	//关闭任务调度

	BaseType_t xReturn;
	vTaskDelay(pdMS_TO_TICKS( 500UL ));		//先延迟一段时间
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 ); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	/*	相关模块初始化	*/
	Led_Init();             //=====初始化与 LED 连接的IO
	Beep_Init();			//=====初始化与 蜂鸣器 连接的IO
	Key_Init();				//=====按键初始化
	Oled_Init();			//=====OLED初始化
	Motor_Init();		    //=====初始化与电机连接的硬件IO接口
	Encoder_Init();       	//=====初始化编码器A
	Uart1_init(115200);	    //=====串口1初始化(用于HC-05蓝牙连接上位机显示波形)
	Uart3_init(115200);     //=====串口3初始化(用于手机遥控)
	Mpu6050_Init();			//=====MPU6050初始化
	Key_EXTI_Init();		//=====按键中断初始化
	MPU6050_EXTI_Init();	//=====MPU6050中断初始化
	xReturn = Task_Init();	//-----创建其他任务
	
	if (xReturn != pdPASS)
		// 进入这里代表其他任务创建失败
		OLED_ShowString(0,8,(u8 *)"Task init fail",16);

	vTaskDelete(NULL); 	// 删除自己

	taskEXIT_CRITICAL();	//打开任务调度
}



int main( void )
{
	BaseType_t xReturn;

 	/* RTOS所需基础硬件初始化 */
	prvSetupHardware();
	
  	/* 创建最初始的任务—————用于初始化小车其他模块以及创建其他任务 */
	xReturn = xTaskCreate(vInitTask, "InitTask", 500, NULL, 10, NULL);
	if (xReturn == pdPASS)
	{
		/* 启动调度器 */
		vTaskStartScheduler();
	}
		
	/* 如果程序运行到了这里就表示出错了, 一般是内存不足 */
	RGB_LED_Red();	//RGB红灯亮起
	BEEP_ON;		//蜂鸣器常鸣
	
	return 0;
}

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
	/* RCC（Reset and Clock Control）复位与时钟控制	Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );	
}
/*-----------------------------------------------------------*/

#ifdef  DEBUG
/* Keep the linker happy. */
void assert_failed( unsigned char* pcFile, unsigned long ulLine )
{
	for( ;; )
	{
	}
}
#endif





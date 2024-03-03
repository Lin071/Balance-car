/*
 * FreeRTOS V202104.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_IDLE_HOOK			0		//设置为1使用空闲钩子（Idle Hook类似于回调函数），0忽略空闲钩子
#define configUSE_TICK_HOOK			0		//设置为1使用时间片钩子（Tick Hook），0忽略时间片钩子
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )		//CPU内核时钟频率，系统CPU主频：stm32f103为72MHz
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )          	//系统滴答，即系统每秒钟滴答的次数。配置为1000，就是把1秒分成了1000份，每一份为 1毫秒
#define configMAX_PRIORITIES		( 5 )   //系统最大的优先级：设置为5时：优先级范围为0-4    FreeRTOS中优先级数值越大，优先级越高
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )  //最小堆栈的设置：用于配置一些任务的堆栈空间的大小，一般用于空闲、定时等一些系统任务中。单位是字
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) )    //设定系统的可用堆栈的总量大小（单位为字），这里定义总计可用的有效的RAM大小为17K
#define configMAX_TASK_NAME_LEN		( 16 )  //用于定义任务名称允许的最大字符串长度。结束符“\0”也包括在内
#define configUSE_TRACE_FACILITY	0		//设置成1表示启动可视化跟踪调试，会激活一些附加的结构体成员和函数
#define configUSE_16_BIT_TICKS		0		//定义为1使用16位计数器 为0使用32位 对于任务最大延时或阻塞时间，16位计数器是262秒，而32位是17179869秒

#define configUSE_PREEMPTION		 1    //调度模式    1：抢占式    0:：合作式
#define configUSE_TIME_SLICING   1
#define configIDLE_SHOULD_YIELD	 1		//当和空闲任务相同优先级的用户任务就绪时 空闲任务是否让出

#define configUSE_MUTEXES           1   //互斥信号量

#define configUSE_TIMERS			 1   /* 使能定时器 */
#define configTIMER_TASK_PRIORITY    31  /* 守护任务的优先级, 尽可能高一些 */
#define configTIMER_QUEUE_LENGTH     5   /* 命令队列长度 */
#define configTIMER_TASK_STACK_DEPTH 32  /* 守护任务的栈大小 */

#define configUSE_COUNTING_SEMAPHORES 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0							//设置成1表示使用协程，0表示不使用协程。如果使用协程，必须在工程中包含croutine.c文件。
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )		//应用程序协程（Co-routines）的有效优先级数目


/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */

//用来设置RTOS内核自己的中断优先级。因为RTOS内核中断不允许抢占用户使用的中断，因此这个宏一般定义为硬件最低优先级。
//用于设置滴答计时器的硬件中断优先级，此次设置为最低优先级-15
#define configKERNEL_INTERRUPT_PRIORITY 		255     // 255=0xFF   
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */


//中断优先级大于等于 11 的中断都会被屏蔽，11 以内的中断则不受 FreeRTOS 管理。
//----即只屏蔽优先级在 12-15，而0-11不可以屏蔽，而且不能在中断中使用freeRTOS的API函数
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */ //191=0xBF


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */

#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15  //没有使用

#endif /* FREERTOS_CONFIG_H */


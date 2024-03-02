/***********************************************************************/
//		    程序说明
//此程序用于：球形机器人(毕设)
//所用MCU芯片：STM32F103C8T6
//作者：071
//
/***********************************************************************/
#include "headfile.h"


int main(void)
{
    delay_init();	        	//=====延时函数初始化	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    Uart1_init(115200);	    //=====串口1初始化(用于HC-05蓝牙连接上位机显示波形)
    Uart3_init(115200);     //=====串口3初始化(用于手机遥控)
    LED_Init();             //=====初始化与 LED 连接的IO
    Key_Init();							//=====按键初始化
    Motor_Init();		    		//=====初始化与电机连接的硬件IO接口 
    Steer_Init();						//=====舵机初始化
    A_Encoder_Init();       //=====初始化编码器A
    B_Encoder_Init();       //=====初始化编码器B
    while(MPU_Init())       //=====初始化MPU6050 
    {
        RGB_LED_green();	//如果一直卡在这，RGB红灯亮起
        delay_ms(200);
    }
    delay_ms(20);
    while(mpu_dmp_init())		//=====MPU6050内置DMP初始化，用于获取姿态角
    {
        RGB_LED_Orange();	//如果一直卡在这，RGB橙灯亮起
        delay_ms(200);
    }  
    OLED_Init();						//=====OLED初始化
    OLED_ShowString(0,8,(u8 *)"hello!",16);		//OLED屏幕显示 Hello
    parameterInit();				//=====参数初始化
    MPU6050_EXTI_Init();		//=====MPU6050中断初始化

    while(1)
    {
        if(task_direct){			//>>>>>10ms执行一次的转向任务
           task_direct = 0;
           directLoop();
        }
        
        if(task_stability){		//>>>>>20ms执行一次的姿态稳定任务
           task_stability = 0;
           stabilityLoop();
        }

        if(task_speed){				//>>>>>50ms执行一次的速度控制任务
           task_speed = 0;
           speedLoop();
        }
        
        if(task_rgb){					//>>>>>100ms执行一次的小灯控制任务
           task_rgb = 0;
           lightControl();
        }

				if(task_oled){				//>>>>>200ms执行一次的OLED屏幕刷新任务
           task_oled = 0;        
        }
    }
}
/*********任务切换位于Exit.c文件夹中的 EXTI9_5_IRQHandler()函数 **********/


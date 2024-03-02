/***********************************************************************/
//		    ����˵��
//�˳������ڣ����λ�����(����)
//����MCUоƬ��STM32F103C8T6
//���ߣ�071
//
/***********************************************************************/
#include "headfile.h"


int main(void)
{
    delay_init();	        	//=====��ʱ������ʼ��	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    Uart1_init(115200);	    //=====����1��ʼ��(����HC-05����������λ����ʾ����)
    Uart3_init(115200);     //=====����3��ʼ��(�����ֻ�ң��)
    LED_Init();             //=====��ʼ���� LED ���ӵ�IO
    Key_Init();							//=====������ʼ��
    Motor_Init();		    		//=====��ʼ���������ӵ�Ӳ��IO�ӿ� 
    Steer_Init();						//=====�����ʼ��
    A_Encoder_Init();       //=====��ʼ��������A
    B_Encoder_Init();       //=====��ʼ��������B
    while(MPU_Init())       //=====��ʼ��MPU6050 
    {
        RGB_LED_green();	//���һֱ�����⣬RGB�������
        delay_ms(200);
    }
    delay_ms(20);
    while(mpu_dmp_init())		//=====MPU6050����DMP��ʼ�������ڻ�ȡ��̬��
    {
        RGB_LED_Orange();	//���һֱ�����⣬RGB�ȵ�����
        delay_ms(200);
    }  
    OLED_Init();						//=====OLED��ʼ��
    OLED_ShowString(0,8,(u8 *)"hello!",16);		//OLED��Ļ��ʾ Hello
    parameterInit();				//=====������ʼ��
    MPU6050_EXTI_Init();		//=====MPU6050�жϳ�ʼ��

    while(1)
    {
        if(task_direct){			//>>>>>10msִ��һ�ε�ת������
           task_direct = 0;
           directLoop();
        }
        
        if(task_stability){		//>>>>>20msִ��һ�ε���̬�ȶ�����
           task_stability = 0;
           stabilityLoop();
        }

        if(task_speed){				//>>>>>50msִ��һ�ε��ٶȿ�������
           task_speed = 0;
           speedLoop();
        }
        
        if(task_rgb){					//>>>>>100msִ��һ�ε�С�ƿ�������
           task_rgb = 0;
           lightControl();
        }

				if(task_oled){				//>>>>>200msִ��һ�ε�OLED��Ļˢ������
           task_oled = 0;        
        }
    }
}
/*********�����л�λ��Exit.c�ļ����е� EXTI9_5_IRQHandler()���� **********/


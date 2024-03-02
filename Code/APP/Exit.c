#include "Exit.h"

u8 task_direct,task_speed,task_stability,task_oled,task_rgb;



/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
����  ֵ���� 
��    �ã�����������MPU6050����INT�ģ�ÿ��MPU6050���������ʱ������INT����Ӧ�ĵ�ƽ�����
					�����������ⲿ�ж���Ϊ�������ڡ�����MPU6050���ݵ�ʵʱ�ԡ�(5ms��һ���ж�)
**************************************************************************/
void MPU6050_EXTI_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	            //�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��PTB5
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
    //////////////////�ⲿ�ж�5���ȼ�����Ҳ����MPU6050 INT���ŵ�����///////////��Ϊ�ǿ����жϣ��ʴ����ȼ�Ӧ����ߡ�
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//ʹ���ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�0�� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
�������ܣ�����Ķ�ʱ�����л�
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��	
				 ��MPU6050�Ĳ���Ƶ�������У����ó�100HZ�����ɱ�֤6050��������10ms����һ�Ρ�
				 ��imv_mpu.h�ļ���26�еĺ궨������޸�(#define DEFAULT_MPU_HZ  (100))
**************************************************************************/
void EXTI9_5_IRQHandler(void) 
{    
    static u8 task10ms,task20ms,task50ms,task100ms,task200ms;
    if(PBin(5)==0)		//�ж��Ƿ�MUP6050�������ж�
    {	
       EXTI->PR=1<<5;     //===���LINE5�ϵ��жϱ�־λ 
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


#include "Motor.h"

/**************************************************************************
//�������ܣ�PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//TIM2_PWM_Init(7199,0);//PWMƵ��=72000/(7199+1)=10Khz
**************************************************************************/
void BMotor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ�ܶ�ʱ��3ʱ��ʹ�� 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIOB����ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}

/**************************************************************************
//�������ܣ�PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//TIM4_PWM_Init(7199,0);//PWMƵ��=72000/(7199+1)=10Khz
**************************************************************************/
void AMotor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
   //���ø�����Ϊ�����������,���TIM3 CH3 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH3  TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

    TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE �����ʹ��	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}

/**************************************************************************
�������ܣ���ʼ����������������
��ڲ�������
����  ֵ����
**************************************************************************/
void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MHZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
    AIN1=0,AIN2=0;
    BIN1=0,BIN2=0;

    //=====��ʼ��PWM 10KHZ,�����������
    AMotor_PWM_Init(7199,0);
    BMotor_PWM_Init(7199,0);

    //ʹ���ֹͣ
    setMotorPwm(0,0);

}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
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


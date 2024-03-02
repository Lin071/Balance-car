#include "Steer.h"

/**************************************************************************
//�������ܣ����PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//Steer_Pwm_Init(1999,719);//PWMƵ��=72000/(1999+1)/(719+1)=50hz
**************************************************************************/
void Steer_Pwm_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	
    //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1 �� TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//<------������仰
    
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}

// �����ʼ��
void Steer_Init(void)
{
    //=====��ʼ��PWM 50HZ,�����������
    Steer_Pwm_Init(1999,719);
    
    // ʹ�����λ���ָ���ֵλ��
    setSteerPwm(FRONTSTEER_MIDIAN,REARSTEER_MIDIAN);
}

// ˫����������ͬ��
void steerSynchronousOut(float pwmSteerOut)
{
    uint16_t tempFrontOut,tempRearOut;
    tempFrontOut = (uint16_t)(FRONTSTEER_MIDIAN - pwmSteerOut);
    tempRearOut = (uint16_t)(REARSTEER_MIDIAN + pwmSteerOut);
    
    setSteerPwm(tempFrontOut,tempRearOut);
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ����������ռ�ձ�(2.5%~12.5%)
����  ֵ����

                ��ǰ

 ǰ���  230 ---  150  --- 80

          80 ---  160  --- 230   ����

                ��β

**************************************************************************/
void setSteerPwm(u16 pwmFront,u16 pwmRear)
{
    // ����ǰ������PWM
    pwmFront = Limiting_uint16(pwmFront,80,230);//����λ��150
    TIM_SetCompare1(TIM1,pwmFront);
    
    // ���ĺ�������PWM
    pwmRear = Limiting_uint16(pwmRear,80,230);  //����λ��160
    TIM_SetCompare4(TIM1,pwmRear);
}


#include "KEY.h"

/**************************************************************************
�������ܣ�LED�ӿڳ�ʼ��
��ڲ������� 
����  ֵ����
**************************************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�ܶ˿�ʱ��
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	//�ⲿ������
    GPIO_Init(GPIOA,&GPIO_InitStructure);					//��ʼ��PA�˿�					
}

/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
uint8_t Key_Scan(void)
{
    static u8 flag_key=1;//�������ɿ���־
    if(flag_key&&PAin(2)==0)
    {
            flag_key=0;
            speedPid.ki += 0.05f;
            return KEY_DOWN;		// ����1����
    }else if(flag_key&&PAin(3)==0)
    {
            flag_key=0;
            speedPid.ki -= 0.01f;
            return KEY_UP;			// ����2����
    }else if(flag_key&&PAin(4)==0)
    {
            flag_key=0;
            speedPid.kp += 1.5f;
            return KEY_REDUCE;	// ����3����
    }else if(flag_key&&PAin(5)==0)
    {
            flag_key=0;
            speedPid.kp -= 0.5f;
            return KEY_ADD;	// ����3����
    }
    else //if(1==KEY)			
    { 
            flag_key=1;
            return 0;//�ް�������
    }
}

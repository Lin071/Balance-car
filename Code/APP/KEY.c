#include "KEY.h"

/**************************************************************************
函数功能：LED接口初始化
入口参数：无 
返回  值：无
**************************************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	//外部已上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure);					//初始化PA端口					
}

/**************************************************************************
函数功能：按键扫描
入口参数：无
返回  值：按键状态 0：无动作 1：单击 
**************************************************************************/
uint8_t Key_Scan(void)
{
    static u8 flag_key=1;//按键按松开标志
    if(flag_key&&PAin(2)==0)
    {
            flag_key=0;
            speedPid.ki += 0.05f;
            return KEY_DOWN;		// 按键1按下
    }else if(flag_key&&PAin(3)==0)
    {
            flag_key=0;
            speedPid.ki -= 0.01f;
            return KEY_UP;			// 按键2按下
    }else if(flag_key&&PAin(4)==0)
    {
            flag_key=0;
            speedPid.kp += 1.5f;
            return KEY_REDUCE;	// 按键3按下
    }else if(flag_key&&PAin(5)==0)
    {
            flag_key=0;
            speedPid.kp -= 0.5f;
            return KEY_ADD;	// 按键3按下
    }
    else //if(1==KEY)			
    { 
            flag_key=1;
            return 0;//无按键按下
    }
}

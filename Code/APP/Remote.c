#include "Remote.h"


/***************************************************************************
//函数功能：串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误 
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
***************************************************************************/
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	uint8_t Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
            Res = USART3->DR; //写DR清除中断标志
//			Res = USART_ReceiveData(USART3);	//读取接收到的数据
//        printf("%d\n",Res);
            Bluetooth_Receive_Parsing(Res);
   } 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
} 

/***************************************************************************
*函  数：void Uart3_putbuff(uint16_t *buff,uint8_t len)
*功  能：串口3发送数组
*参  数：buff为要发送数组的地址      len为数组长度
*返回值：无 	
***************************************************************************/
void Uart3_putbuff(uint16_t *buff,uint8_t len)
{
    while(len--)
    {
        USART_SendData(USART3,*buff);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
        buff++;
    }
}


/***************************************************************************
*函  数：void uart_init(u32 bound)
*功  能：串口3初始化函数
*参  数：bound为设置串口的波特率
*返回值：无 	
***************************************************************************/	  
void Uart3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	//USART3_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART3_RX	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
    //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure);       //初始化串口3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART3, ENABLE);                    //使能串口3 
}


// 串口发送字符串
void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
		i++;
	}
}

char sendList[1];

// 接收遥控器命令并解析
void Bluetooth_Receive_Parsing(u8 data)
{
    u8 temp;
    temp = data & 0xF0;
    
    // 判断是按键还是开关
    if(temp == 0x10){ //按键按下
        switch(data)
		{
				case 0x11: //左前行驶
                    
                    break;
				case 0x12: //前进
                    speedPid.out = 2500 + speedIncrementalTemp;
                    break;
				case 0x13: //右前行驶
                    
                    break;
				case 0x14: //左转
                    directPid.out = 2500;
                    break;
				case 0x15: //蜂鸣器打开
                    BEEP_ON;
                    break;
				case 0x16: //右转
                    directPid.out = -2500;
                    break;
				case 0x17: //左后行驶
                    
                    break;
				case 0x18: //后退
                    speedPid.out = -2500 - speedIncrementalTemp;
                    break;
				case 0x19: //右后行驶
                   
                    break;
				case 0x1A: //速度+
                    speedIncrementalTemp += 100;
                    break;
				case 0x1B: 
                    break;
				case 0x1C: //速度-
                    speedIncrementalTemp -= 100;
                    break;
		}
    }else if(temp == 0x20){ //按键松开
        switch(data)
		{
				case 0x22: //小车前进和后退停止      
				case 0x28:
                    speedPid.out = 0;
                    break;
				case 0x24:
                case 0x26:
                    directPid.out = 0;
                    break;
				case 0x23:
				case 0x27:
				case 0x21:
				case 0x29:
                    
                    break;    
                
				case 0x25: //蜂鸣器关闭
                    BEEP_OFF;
                    break;
				
				case 0x2A:
                    break;
				case 0x2B:
                    break;
				case 0x2C:
                    break;
		}
    }else if(temp == 0x30){ //开关正档
         switch(data)
		{
				case 0x31:      
                    flag.rgbState = RGB_RUNNING;
                    break;
				case 0x32:
                    flag.rgbState = RGB_RUNNING;
                    break;
				case 0x33:
                    flag.rgbState = RGB_FLY;
                    break;
				case 0x34:
                    flag.rgbState = RGB_RED;
                    break;
				case 0x35:
                    flag.rgbState = RGB_BLUE;
                    break;
				case 0x36:
                    flag.rgbState = RGB_YELLOW;
                    break;
				case 0x37:
                    flag.rgbState = RGB_WHITE;
                    break;
				case 0x38:
                    break;
				case 0x39:
                    break;
				case 0x3A:
                    break;
				case 0x3B:
                    break;
				case 0x3C:
                    break;
		}
    }else if(temp == 0x40){ //开关反档
         switch(data)
		{
				case 0x41:      
				case 0x42:
				case 0x43:
				case 0x44:
				case 0x45:
				case 0x46:
				case 0x47:
                    flag.rgbState = RGB_OFF;
                    break;
				case 0x48:
                    break;
				case 0x49:
                    break;
				case 0x4A:
                    break;
				case 0x4B:
                    break;
				case 0x4C:
                    break;
		}
    }
}

#include "Remote.h"


/***************************************************************************
//�������ܣ�����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
***************************************************************************/
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	uint8_t Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
            Res = USART3->DR; //дDR����жϱ�־
//			Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
//        printf("%d\n",Res);
            Bluetooth_Receive_Parsing(Res);
   } 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
} 

/***************************************************************************
*��  ����void Uart3_putbuff(uint16_t *buff,uint8_t len)
*��  �ܣ�����3��������
*��  ����buffΪҪ��������ĵ�ַ      lenΪ���鳤��
*����ֵ���� 	
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
*��  ����void uart_init(u32 bound)
*��  �ܣ�����3��ʼ������
*��  ����boundΪ���ô��ڵĲ�����
*����ֵ���� 	
***************************************************************************/	  
void Uart3_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	//USART3_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART3_RX	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure);       //��ʼ������3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
}


// ���ڷ����ַ���
void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//�����ַ�ǰ��ձ�־λ������ȱʧ�ַ����ĵ�һ���ַ���
	while(DataString[i] != '\0')												//�ַ���������
	{
		USART_SendData(USARTx,DataString[i]);									//ÿ�η����ַ�����һ���ַ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//�ȴ����ݷ��ͳɹ�
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//�����ַ�����ձ�־λ
		i++;
	}
}

char sendList[1];

// ����ң�����������
void Bluetooth_Receive_Parsing(u8 data)
{
    u8 temp;
    temp = data & 0xF0;
    
    // �ж��ǰ������ǿ���
    if(temp == 0x10){ //��������
        switch(data)
		{
				case 0x11: //��ǰ��ʻ
                    
                    break;
				case 0x12: //ǰ��
                    speedPid.out = 2500 + speedIncrementalTemp;
                    break;
				case 0x13: //��ǰ��ʻ
                    
                    break;
				case 0x14: //��ת
                    directPid.out = 2500;
                    break;
				case 0x15: //��������
                    BEEP_ON;
                    break;
				case 0x16: //��ת
                    directPid.out = -2500;
                    break;
				case 0x17: //�����ʻ
                    
                    break;
				case 0x18: //����
                    speedPid.out = -2500 - speedIncrementalTemp;
                    break;
				case 0x19: //�Һ���ʻ
                   
                    break;
				case 0x1A: //�ٶ�+
                    speedIncrementalTemp += 100;
                    break;
				case 0x1B: 
                    break;
				case 0x1C: //�ٶ�-
                    speedIncrementalTemp -= 100;
                    break;
		}
    }else if(temp == 0x20){ //�����ɿ�
        switch(data)
		{
				case 0x22: //С��ǰ���ͺ���ֹͣ      
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
                
				case 0x25: //�������ر�
                    BEEP_OFF;
                    break;
				
				case 0x2A:
                    break;
				case 0x2B:
                    break;
				case 0x2C:
                    break;
		}
    }else if(temp == 0x30){ //��������
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
    }else if(temp == 0x40){ //���ط���
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

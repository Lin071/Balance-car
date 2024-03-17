#include "mpuiic.h"

// ʵ��΢�뼶��ʱ
static void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, reload, tcnt = 0;

    reload = SysTick->LOAD;                    // ��ȡ��װ�ؼĴ���ֵ
    ticks = nus * (SystemCoreClock / 1000000); // ����ʱ��ֵ
    told = SysTick->VAL;                       // ��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ��

    while (1)
    {
        tnow = SysTick->VAL; // ��ȡ��ǰ��ֵ�Ĵ���ֵ
        if (tnow != told)    // ��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
        {
            if (tnow < told) {        // ��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0
                tcnt += told - tnow; // ����ֵ=��ʼֵ-��ǰֵ
            } else {                              // ��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
                tcnt += reload - tnow + told; // ����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ  ���Ѵӿ�ʼֵ�Ƶ�0��
            }

            told = tnow; // ���¿�ʼֵ
            if (tcnt >= ticks) {
                break; // ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
            }
        }
    }
}


//MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	delay_us(2);
}

//�ı�IO���������������
void IIC_SDA_DDR(GPIOMode_TypeDef mode)
{
		GPIO_InitTypeDef  GPIO_InitStructure; 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 // �˿�����
		GPIO_InitStructure.GPIO_Mode = mode; 
		if(mode == GPIO_Mode_Out_PP)
		{
					 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		}
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{			
     GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PB�˿�ʱ��
    
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	//�˿�����
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
     GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}
//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //sda�����
	MPU_IIC_SDA=1;	  	  
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda�����
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	MPU_IIC_SCL=1;  
	MPU_IIC_SDA=1;//����I2C���߽����ź�
	MPU_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA����Ϊ����  
	MPU_IIC_SDA=1;MPU_IIC_Delay();	   
	MPU_IIC_SCL=1;MPU_IIC_Delay();	 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MPU_SDA_OUT(); 	    
    MPU_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MPU_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		MPU_IIC_SCL=1;
		MPU_IIC_Delay(); 
		MPU_IIC_SCL=0;	
		MPU_IIC_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MPU_IIC_SCL=0; 
        MPU_IIC_Delay();
		MPU_IIC_SCL=1;
        receive<<=1;
        if(MPU_READ_SDA)receive++;   
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK   
    return receive;
}

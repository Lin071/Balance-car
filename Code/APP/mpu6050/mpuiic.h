#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "sys.h"
#include <stdint.h>   		   
					 
					 
//IO��������
#define MPU_SDA_IN()   {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}
#define MPU_SDA_OUT()  {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}


//IO��������	 
#define MPU_IIC_SCL     PBout(4) //SCL
#define MPU_IIC_SDA     PBout(3) //SDA	
#define MPU_READ_SDA   	PBin(3) //SDA	


//IIC���в�������
void IIC_SDA_DDR(GPIOMode_TypeDef mode);
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�


void IMPU_IC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t MPU_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif

















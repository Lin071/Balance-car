#ifndef _OLED_H_
#define _OLED_H_

#include <stdint.h>
#include "stm32f10x_gpio.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void Oled_Init(void);
void OLED_Clear(void);

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	 

//���º�����Ϊ�ڲ�����
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void OLED_IIC_Wait_Ack(void);
void OLED_Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Write_IIC_Command(unsigned char IIC_Command);
void OLED_Write_IIC_Data(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned dat,unsigned cmd);



#endif




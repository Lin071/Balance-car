#ifndef __USART_H
#define __USART_H
#include "headfile.h" 

//����봮���жϽ��գ��벻Ҫע�����º궨��
#define USART_REC_LEN  		200  	//�����������ֽ��� 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	




void Uart1_init(u32 bound);
void Uart1_putbuff(uint16_t *buff,uint8_t len);



#endif


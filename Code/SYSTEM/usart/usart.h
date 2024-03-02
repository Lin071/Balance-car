#ifndef __USART_H
#define __USART_H
#include "headfile.h" 

//如果想串口中断接收，请不要注释以下宏定义
#define USART_REC_LEN  		200  	//定义最大接收字节数 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	




void Uart1_init(u32 bound);
void Uart1_putbuff(uint16_t *buff,uint8_t len);



#endif


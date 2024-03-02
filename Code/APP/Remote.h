#ifndef _REMOTE_H_
#define _REMOTE_H_
#include "headfile.h"

void Uart3_init(u32 bound);
void Uart3_putbuff(uint16_t *buff,uint8_t len);
void USART_SendString(USART_TypeDef* USARTx, char *DataString);

void Bluetooth_Receive_Parsing(u8 data);


#endif

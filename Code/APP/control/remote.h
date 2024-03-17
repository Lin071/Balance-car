#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <stdint.h>
#include "stm32f10x_usart.h"



// 定义结构体，存储遥控相关标志
typedef struct 
{
    uint8_t rgbState;
}Remote;


extern Remote remoteState;


void Bluetooth_Receive_Parsing(uint8_t data);

void Uart1_init(uint32_t bound);
void Uart3_init(uint32_t bound);

void USART_SendString(USART_TypeDef* USARTx, char *DataString);




#endif

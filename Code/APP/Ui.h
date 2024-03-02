#ifndef _UI_H_
#define _UI_H_
#include "headfile.h"

typedef struct
{
	uint8_t Mouse_Now;
	uint8_t Mouse_Last;
	char str[2];
	uint8_t Key_Get;
}Page_Flag;



void Page_Init(void);
void Pages_Show(void);
void Page_Main_Menu(void);
void Speed_PID_Show(void);
void Dir_PID_Show(void);
void Print_To_Oled(uint8_t j);


#endif



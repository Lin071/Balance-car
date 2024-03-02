#ifndef _KEY_H_
#define _KEY_H_
#include "headfile.h"

#define KEY_UP     1
#define KEY_DOWN   2
#define KEY_ADD    3
#define KEY_REDUCE 4


void Key_Init(void);
uint8_t Key_Scan(void);

#endif



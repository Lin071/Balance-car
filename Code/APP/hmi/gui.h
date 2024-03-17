#ifndef _UI_H_
#define _UI_H_

#include <stdint.h>


// 宏定义这个函数指针数组的大小
#define ARR_SIZE    4


// 声明函数指针类型
typedef void (*FunctionPtr)(void);


// 向外部声明
extern FunctionPtr pageFunctions[ARR_SIZE];
extern FunctionPtr pPage;




void Page_Init(void);
void IMU_PID_Show(void);
void Balance_PID_Show(void);
void Dir_PID_Show(void);
void Speed_PID_Show(void);


#endif



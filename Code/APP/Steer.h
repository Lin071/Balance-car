#ifndef _STEER_H_
#define _STEER_H_
#include "headfile.h"

#define FRONTSTEER_MIDIAN 150
#define REARSTEER_MIDIAN 160

void Steer_Pwm_Init(u16 arr,u16 psc);
void Steer_Init(void);

void steerSynchronousOut(float pwmSteerOut);
void setSteerPwm(u16 pwmFront,u16 pwmRear);

#endif



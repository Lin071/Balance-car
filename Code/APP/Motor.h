#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "headfile.h"


#define AIN2   PBout(12)
#define AIN1   PBout(13)
#define BIN1   PBout(14)
#define BIN2   PBout(15)


void AMotor_PWM_Init(u16 arr,u16 psc);
void BMotor_PWM_Init(u16 arr,u16 psc);
void Motor_Init(void);

void setMotorPwm(int16_t moto1,int16_t moto2);

#endif

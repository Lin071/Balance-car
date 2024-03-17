#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdint.h>
#include "sys.h"

#define AIN2   PBout(12)
#define AIN1   PBout(13)
#define BIN1   PBout(14)
#define BIN2   PBout(15)


void Motor_Init(void);

void setMotorPwm(int16_t moto1,int16_t moto2);

#endif

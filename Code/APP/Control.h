#ifndef _CONTROL_H
#define _CONTROL_H
#include "headfile.h"

#define RGB_OFF      0 
#define RGB_RUNNING  1
#define RGB_FLY      2
#define RGB_RED      3
#define RGB_BLUE     4
#define RGB_YELLOW   5
#define RGB_WHITE    6


#define CARSTOP_MODE       0
#define FORWARD_MODE       1
#define BACK_MODE          2
#define RIGHTROUND_MODE    3
#define LEFTROUND_MODE     4
#define AVOIDOBSTACLE_MODE 5


typedef struct{
    float expect;
    float kp;
    float ki;
    float kd;
    float integral;
    float error;
    float errorLast;
    float out;
}PID;

typedef struct{
    u8 rgbState;
    u8 remoteState;
}Flag;




// 标志位变量
extern Flag flag;
// pid变量
extern PID speedPid,stabilityPid,directPid;
// 其他控制变量
extern int16_t leftSpeed,rightSpeed;
extern float totalSpeed;
extern int16_t leftWheelOut,rightWheelOut,stabilityOut;
// 遥控模式下的变量
extern int16_t speedIncrementalTemp,deviateSpeed;


extern float angle_z,angularVelocity_z; // 单位：°/s;
extern float pitch,roll,yaw;


void directLoop(void);
void speedLoop(void);
void stabilityLoop(void);
void lightControl(void);
void parameterInit(void);

void pidCalculate2(float *value,float *expect,PID *pid,float limit);
void pidCalculate(float *value,float *expect,PID *pid,float limit);
void differentialFirstPidCalculate(float *value,float *expect,PID *pid);

#endif

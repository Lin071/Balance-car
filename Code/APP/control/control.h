#ifndef _CONTROL_H
#define _CONTROL_H
#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"






/* bit0: UP     上一页
 * bit1: DOWN   下一页
 */
#define UP     (1<<0)
#define DOWN   (1<<1)



typedef struct
{
    float expect;
    float Kp;
    float Ki;
    float Kd;
    float error;
    float errorLast;
    float integral;
    int16_t out;
}PID;


extern PID balancePid, directionPid, speedPid;
extern float pitch, roll, yaw;
extern short gx, gy, gz;


extern TaskHandle_t xBalanceTaskHandle;
extern EventGroupHandle_t xEventGroup;     // 事件组句柄 
extern QueueHandle_t xQueue;



BaseType_t Task_Init(void);


#endif

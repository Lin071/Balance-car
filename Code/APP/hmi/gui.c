#include "gui.h"

#include "key.h"
#include "oled.h"
#include <stdint.h>
#include <stdio.h>
#include "control.h"



// 定义一个函数指针
FunctionPtr pPage;  

// 定义函数指针数组
FunctionPtr pageFunctions[ARR_SIZE] = {
    IMU_PID_Show,
    Balance_PID_Show,
    Dir_PID_Show,
    Speed_PID_Show
};


// 定义一个二维字符数组，用于存储需要显示的字符串
static char list[10][20]={0};


//打印到屏幕
static void Print_To_Oled(uint8_t j)
{
    uint8_t i = 0;
    
    for(; i < j; i++)
       OLED_ShowString(8, i, (uint8_t *)list[i], 12);
}


//页面相关初始化
void Page_Init(void)
{
	pPage = IMU_PID_Show;
	OLED_Clear();
}


//IMU页面
void IMU_PID_Show(void)
{
    uint8_t cnt = 0;
    sprintf(list[cnt++],"Pitch:%2.2f", pitch);
    sprintf(list[cnt++],"roll:%2.2f", roll);
    sprintf(list[cnt++],"yaw:%2.2f", yaw);
    Print_To_Oled(cnt);
}



//直立环菜单页面
void Balance_PID_Show(void)
{
    uint8_t cnt = 0;
    sprintf(list[cnt++],"BAL_Ex:%2.2f", balancePid.expect);
    sprintf(list[cnt++],"BAL_Kp:%2.2f", balancePid.Kp);
    sprintf(list[cnt++],"BAL_Ki:%2.2f", balancePid.Ki);
    sprintf(list[cnt++],"BAL_Kd:%2.2f", balancePid.Kd);
    Print_To_Oled(cnt);
}


//方向环菜单页面
void Dir_PID_Show(void)
{
    uint8_t cnt = 0;
    sprintf(list[cnt++],"DIR_Ex:%2.2f", directionPid.expect);
    sprintf(list[cnt++],"DIR_Kp:%2.2f", directionPid.Kp);
    sprintf(list[cnt++],"DIR_Ki:%2.2f", directionPid.Ki);
    sprintf(list[cnt++],"DIRL_Kd:%2.2f", directionPid.Kd);
    Print_To_Oled(cnt);
}


//速度环菜单页面
void Speed_PID_Show(void)
{
    uint8_t cnt = 0;
    sprintf(list[cnt++],"SPE_Ex:%2.2f", speedPid.expect);
    sprintf(list[cnt++],"SPE_Kp:%2.2f", speedPid.Kp);
    sprintf(list[cnt++],"SPE_Ki:%2.2f", speedPid.Ki);
    sprintf(list[cnt++],"SPE_Kd:%2.2f", speedPid.Kd);
    sprintf(list[cnt++],"integ:%2.2f", speedPid.integral);
    Print_To_Oled(cnt);
}





#include "control.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"


#include "led.h"
#include "mpu6050.h"
#include "motor.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "oled.h"
#include "gui.h"
#include "remote.h"


// ==定义相关全局变量
PID balancePid, directionPid, speedPid;
float pitch, roll, yaw;
short gx, gy, gz;

// ===定义相关外部所需句柄
TaskHandle_t xBalanceTaskHandle;    // 直立环任务句柄
EventGroupHandle_t xEventGroup;     // 事件组句柄 
QueueHandle_t xQueue;               // 队列句柄


// ==定义任务句柄
static TaskHandle_t xLedTaskHandle;
static TaskHandle_t xRgbTaskHandle;
static TaskHandle_t xOledTaskHandle;
static TaskHandle_t xRemoteTaskHandle;
static TaskHandle_t xSpeedTaskHandle;



/***************************************************************************
*函  数：static void parameterInit(void)
*功  能：PID等参数初始化
*参  数：无
*返回值：无
***************************************************************************/
static void parameterInit(void)
{
    // PID系数初始化
    balancePid.expect = -7.5f;
    balancePid.Kp = 465.0f;       // 0 ~ 700 
    balancePid.Ki = 0.0f;
    balancePid.Kd = -0.55f;

    directionPid.expect = 0;
    directionPid.Kp = 0.65f;
    directionPid.Ki = 0.0f;
    directionPid.Kd = 0.0f;

    speedPid.expect = 0;
    speedPid.Kp = 13.0f;
    speedPid.Ki = 0.065f;
    speedPid.Kd = 0.0f;
}


/***************************************************************************
*函  数：void vApplicationIdleHook(void)
*功  能：空闲任务钩子函数————(优先级最低：0     该任务不可以阻塞)
        1、处理被删除的任务
        2、刷新OLED屏幕
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
void vApplicationIdleHook(void)
{
    OLED_Clear();
    pPage();        // 刷新OLED屏幕
}


/***************************************************************************
*函  数：static void vLedTask(void *pvParameters)
*功  能：LED灯任务，闪烁LED灯，提示小车正常运行
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vLedTask(void *pvParameters)
{
    const TickType_t xDelayTicks = pdMS_TO_TICKS(500UL);
    uint8_t cnt = 0;
    
    while(1)
    {
        cnt++;
        if(cnt%2 == 0)      // 翻转LED
            LED_OFF;
        else
            LED_ON;
        
        /* 延时 */
        vTaskDelay(xDelayTicks);
    }
}



/***************************************************************************
*函  数：static void vRgbTask(void *pvParameters)
*功  能：RGB灯任务，控制RGB灯的行为，使小车更具趣味性
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vRgbTask(void *pvParameters)
{
    const TickType_t xDelayTicks = pdMS_TO_TICKS(200UL);
    
    while(1)
    {
        // 根据rgbState标志位选择RGB状态
        switch(remoteState.rgbState)
        {
            case RGB_OFF:
                RGB_LED_Off();
                break;
            case RGB_FLY:
                RGB_LED_FLY();
                break;
            case RGB_RUNNING:
                RGB_LED_Runing();
                break;
            case RGB_RED:
                RGB_LED_Red();
                break;
            case RGB_BLUE:
                RGB_LED_Blue();
                break;
            case RGB_YELLOW:
                RGB_LED_Yellow();
                break;
            case RGB_WHITE:
                RGB_LED_White();
                break;     
        }

        /* 延时 */
        vTaskDelay(xDelayTicks);
    }
}



/***************************************************************************
*函  数：static void vOledTask(void *pvParameters)
*功  能：OLED显示控制任务，用于控制OLED显示的内容
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vOledTask(void *pvParameters)
{
    const TickType_t xDelayTicks = pdMS_TO_TICKS(20UL);
    int8_t i = 0;
    EventBits_t xReturnBit;
    Page_Init();    // 初始化屏幕显示页
    
    while(1)
    {
        /* 延时 */
        vTaskDelay(xDelayTicks);

        /* 等待某一个按键，获取事件后清除标志位，无限期阻塞 */
		xReturnBit = xEventGroupWaitBits(xEventGroup, UP|DOWN, pdTRUE, pdFALSE, portMAX_DELAY);

        // 根据返回值判断哪个按键按下
        if (xReturnBit == UP)
        {
            i--;
            i = i < 0 ? 0 : i;      // 限幅
        } else if (xReturnBit == DOWN)
        {
            i++;
            i = i > (ARR_SIZE-1) ? (ARR_SIZE-1) : i;    // 限幅
        }

        pPage = pageFunctions[i];   // 更新页面
    }
}


/***************************************************************************
*函  数：static void vRemoteTask(void *pvParameters)
*功  能：解析遥控命令任务，等待串口3发送数据至队列，不断读取队列数据
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vRemoteTask(void *pvParameters)
{
    uint8_t receivedValue;
    BaseType_t xStatus;

    while(1)
    {
        /* 读取从手机遥控端发送过来的消息，即读取队列
         * 从 xQueue 队列中读取数据至 receivedValue 变量，如果队列没有数据，则无限阻塞
         */
        xStatus = xQueueReceive( xQueue, &receivedValue, portMAX_DELAY);

        // 判断是否读到了数据
        if (xStatus == pdPASS)
        {   
            // 带入解析
            Bluetooth_Receive_Parsing(receivedValue);
        }
    }
}



/***************************************************************************
*函  数：static void vBalanceTask(void *pvParameters)
*功  能：直立环控制任务
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vBalanceTask(void *pvParameters)
{
    parameterInit();

    while(1)
    {
        /* 阻塞等待MPU6050中断任务通知，被通知后将通知值清零 */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        MPU_Get_Gyroscope(&gx, &gy, &gz);         //===得到角速度的数据
        mpu_dmp_get_data(&pitch, &roll, &yaw);    //===得到欧拉角（姿态角）的数据

        // 直立环代码...
        balancePid.error = balancePid.expect - pitch;
        balancePid.out = (int16_t)(balancePid.Kp*balancePid.error + balancePid.Kd*gy);

        // 方向环代码...
        directionPid.error = directionPid.expect - gz;
        directionPid.out = (int16_t)(directionPid.Kp*directionPid.error);

        /* 改变左右两轮占空比 */
        // setMotorPwm(balancePid.out + speedPid.out - directionPid.out, balancePid.out + speedPid.out + directionPid.out);
        setMotorPwm(0, 0);
    }
}


/***************************************************************************
*函  数：static void vSpeedTask(void *pvParameters)
*功  能：速度环控制任务
*参  数：pvParameters 为创建任务时所传递的参数
*返回值：无
***************************************************************************/
static void vSpeedTask(void *pvParameters)
{
    const TickType_t xDelayTime = pdMS_TO_TICKS( 10UL );
    TickType_t xLastWakeTime = xTaskGetTickCount();     // 获得当前的Tick Count
    int16_t leftSpeed, rightSpeed;
    float totalSpeed, totalSpeedLast;

    while(1)
    {
        getSpeed(&leftSpeed, &rightSpeed);      // 获取左右两轮编码器数值作为车速

        totalSpeedLast = (float)(leftSpeed+rightSpeed);
        totalSpeed *= 0.8;
        totalSpeed += totalSpeedLast*0.2;   // 一阶低通滤波
        speedPid.error = totalSpeed - speedPid.expect;
        speedPid.integral += totalSpeed;    // 积分
        speedPid.integral = speedPid.integral > 15000 ? 15000 : speedPid.integral;      // 积分限幅
        speedPid.integral = speedPid.integral < -15000 ? -15000 : speedPid.integral;
        // 计算速度环PID...
        speedPid.out = (int16_t)(speedPid.Kp*speedPid.error + speedPid.Ki*speedPid.integral);
        if (pitch < -40 || pitch > 40)
        {
            speedPid.integral = 0;
        }
        
        /* 周期延时 */
        vTaskDelayUntil(&xLastWakeTime, xDelayTime);
    }
}


/***************************************************************************
*函  数：BaseType_t Task_Init(void)
*功  能：任务初始化————创建小车所用到的一些任务
*参  数：无
*返回值：pdFAIL 表示失败        pdPASS 表示成功
***************************************************************************/
BaseType_t Task_Init(void)
{
    int8_t cnt = 0;
    BaseType_t xReturn[6];

    OLED_ShowString(0, 8, (u8 *)"Task init...", 16);

    /* 创建队列: 长度为5，数据大小为2字节(存放一个整数) */
    xQueue = xQueueCreate( 5, sizeof( int16_t ) );
    if (xQueue == NULL) return pdFAIL;

    /* 创建事件组 */
	  xEventGroup = xEventGroupCreate();
    if(xEventGroup == NULL)   return pdFAIL;
    
    xReturn[cnt++] = xTaskCreate(vLedTask,"LedTask",100, NULL, 2,&xLedTaskHandle);
    xReturn[cnt++] = xTaskCreate(vRgbTask,"RgbTask",100, NULL, 2,&xRgbTaskHandle);
    xReturn[cnt++] = xTaskCreate(vOledTask,"OledTask",200, NULL, 3,&xOledTaskHandle);
    xReturn[cnt++] = xTaskCreate(vRemoteTask,"RemoteTask",200, NULL, 3,&xRemoteTaskHandle);
    xReturn[cnt++] = xTaskCreate(vBalanceTask, "vBalanceTask", 500, NULL, 5, &xBalanceTaskHandle);
    xReturn[cnt++] = xTaskCreate(vSpeedTask, "vSpeedTask", 300, NULL, 4, &xSpeedTaskHandle);

    while (cnt > 0)
        if (xReturn[--cnt] != pdPASS)  return pdFAIL;

	return pdPASS;
}



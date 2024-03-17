#ifndef _headfile_h
#define _headfile_h


//--------freeRTOS------
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#ifndef delay_ms
		// 定义一个宏，用于将毫秒转换为 FreeRTOS 滴答数
		#define delay_ms(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#endif



#endif

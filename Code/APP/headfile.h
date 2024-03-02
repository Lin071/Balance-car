#ifndef _headfile_h
#define _headfile_h


#include "stm32f10x.h"
#include "misc.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

//--------函数库--------
#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_can.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"

//--------系统中断处理函数------
#include "stm32f10x_it.h"

//--------系统应用函数库--------
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "LED.h"
#include "KEY.h"
#include "Motor.h"
#include "Encoder.h"
#include "OLED.h"
#include "Exit.h"
#include "Remote.h"
#include "Control.h"
#include "Mymath.h"
#include "Steer.h"
#include "Ui.h"

#endif

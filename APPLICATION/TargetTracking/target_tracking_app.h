/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#ifndef _TARGET_TRACKING_APP_H_
#define _TARGET_TRACKING_APP_H_

#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "varistor.h"
#include "ms5837.h"

#include "rflink.h"
#include "uart6_printf.h"
#include "BuffPrintf.h"
#include "robotstate.h"

// 任务定义
#define TARGET_TRACKING_APP_TASK_PRIO 8
#define TARGET_TRACKING_APP_STK_SIZE 256
/* The Timer Ticks frequency has been setted to 1000Hz */
#define TARGET_TRACKING_APP_TIMER_PERIOD_MS 10
#define TARGET_TRACKING_APP_TIMER_PERIOD_TICKS (((uint32_t)TARGET_TRACKING_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)


void target_tracking_app_init(void);
void target_tracking_app_stop(void);
void target_tracking_app_resume(void);

#endif
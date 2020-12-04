/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _DATA_STORAGE_APP_H_
#define _DATA_STORAGE_APP_H_

#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"


#include "malloc.h" 
#include "ff.h"
#include "exfuns.h"
#include "fattester.h"

#include "BuffPrintf.h"
#include "robotstate.h"
#include "config.h"

// 任务定义
#define DATA_STORAGE_APP_TASK_PRIO 10
#define DATA_STORAGE_APP_STK_SIZE 256
/* The Timer Ticks frequency has been setted to 1000Hz */
#define DATA_STORAGE_APP_TIMER_PERIOD_MS 20
#define DATA_STORAGE_APP_TIMER_PERIOD_TICKS (((uint32_t)DATA_STORAGE_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)


void data_storage_app_init(void);
void data_storage_app_stop(void);
void data_storage_app_resume(void);
void set_savefile_name(uint8_t ftype, unsigned char* fname);

#endif
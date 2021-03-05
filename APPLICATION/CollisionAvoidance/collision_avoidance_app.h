/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#ifndef _COLLISION_AVOIDANCE_APP_H_
#define _COLLISION_AVOIDANCE_APP_H_

// 系统头文件
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
//机器人状态
#include "robotstate.h"


// 定义任务
#define COLLISION_AVOIDANCE_APP_TASK_PRIO 5  // 设置任务优先级,一般最少都有256个优先级，数字越小代表的优先级越高
#define COLLISION_AVOIDANCE_APP_STK_SIZE 256 // 设置任务堆栈大小（这里的1个代表4字节）
#define COLLISION_AVOIDANCE_APP_TIMER_PERIOD_MS 10 // 设置运行周期，这里说明期望这个任务每隔10ms运行一次
#define COLLISION_AVOIDANCE_APP_TIMER_PERIOD_TICKS (((uint32_t)COLLISION_AVOIDANCE_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ) /* 定时器时钟频率被设置为了 1000Hz */

// 任务初始化函数
void collision_avoidance_app_init(void);
// 任务暂停函数
void collision_avoidance_app_stop(void);
// 任务恢复运行函数
void collision_avoidance_app_resume(void);

#endif
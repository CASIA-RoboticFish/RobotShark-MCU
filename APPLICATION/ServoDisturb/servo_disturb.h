/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __SERVO_DISTURB_H_
#define __SERVO_DISTURB_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "math.h"
#include "pwm.h"
#include "BuffPrintf.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#include "os_cfg_app.h"
#endif


/********************************************************************************
*
* 任务配置区域
*
********************************************************************************/

// 配置横滚扰动任务
#define ROLL_SERVO_DISTURB_TASK_PRIO 5
#define ROLL_SERVO_DISTURB_STK_SIZE 256
void roll_servo_disturb_task(void *p_arg);
void roll_servo_disturb_task_init();
#define ROLL_SERVO_TIMER_PERIOD_MS 10
#define ROLL_SERVO_TIMER_PERIOD_TICKS (((uint32_t)ROLL_SERVO_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)


// 配置偏航扰动任务
#define YAW_SERVO_DISTURB_TASK_PRIO 5
#define YAW_SERVO_DISTURB_STK_SIZE 256
void yaw_servo_disturb_task(void *p_arg);
void yaw_servo_disturb_task_init();
#define YAW_SERVO_TIMER_PERIOD_MS 10
#define YAW_SERVO_TIMER_PERIOD_TICKS (((uint32_t)YAW_SERVO_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)


#endif
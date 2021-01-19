/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _SWIM_CONTROL_APP_H_
#define _SWIM_CONTROL_APP_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
#include "BuffPrintf.h"
//#include "cpg.h"

#include "tail_servo_control.h"
#include "pectoral_servo_control.h"

#include "robotstate.h"

// 任务定义
#define SWIM_CONTROL_APP_TASK_PRIO 6
#define SWIM_CONTROL_APP_STK_SIZE 256
/* The Timer Ticks frequency has been setted to 1000Hz */
#define SWIM_CONTROL_APP_TIMER_PERIOD_MS 10
#define SWIM_CONTROL_APP_TIMER_PERIOD_TICKS (((uint32_t)SWIM_CONTROL_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

// 尾巴相关函数
void set_tail_angle(float angle);
void set_tail_amp(float amp);
void set_tail_freq(float freq);
void set_tail_offset(float offset);

// 胸鳍相关函数
void set_pect_fin_angle(float angle, uint8_t channel);
// 左侧胸鳍
void set_leftpectfin_angle(float angle);
void set_leftpectfin_amp(float amp);
void set_leftpectfin_freq(float freq);
void set_leftpectfin_offset(float offset);
// 右侧胸鳍
void set_rightpectfin_angle(float angle);
void set_rightpectfin_amp(float amp);
void set_rightpectfin_freq(float freq);
void set_rightpectfin_offset(float offset);

// 任务初始化函数
void swim_control_app_init(void);
// 任务启动函数
void swim_control_start(void);
void swim_control_tail_start(void);
void swim_control_leftpectfin_start(void);
void swim_control_rightpectfin_start(void);
// 任务暂停函数
void swim_control_stop(void);
void swim_control_tail_stop(void);
void swim_control_leftpect_stop(void);
void swim_control_rightpect_stop(void);
// 任务强制停止函数
void swim_control_forcestop(void);
#endif 

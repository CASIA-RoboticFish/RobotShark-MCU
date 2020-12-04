/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __FUZZY_PID_CONTROL_H_
#define __FUZZY_PID_CONTROL_H_

#include "math.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "motor.h"
#include "JY901.h"
#include "BuffPrintf.h"

//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#include "os_cfg_app.h"
#endif

#define RAD_TO_DEG 57.2975f
/********************************************************************************
*
* 外部变量申明区域
*
********************************************************************************/
extern struct SIMUDATA stcIMU[2];

/********************************************************************************
*
* 模糊控制专区
*
********************************************************************************/
#define NL   -3
#define NM	 -2
#define NS	 -1
#define ZE	 0
#define PS	 1
#define PM	 2
#define PL	 3

static const float fuzzyRuleKp[7][7]={
	PL,	PL,	PM,	PM,	PS,	PS,	ZE,
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	NS,	NS,	NM,	NM,	NL,	NL
};
 
static const float fuzzyRuleKi[7][7]={
	NL,	NL,	NL,	NM,	NM,	ZE,	ZE,
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PL,	PL,	PL
};
 
static const float fuzzyRuleKd[7][7]={
	PS,	PS,	ZE,	ZE,	ZE,	PL,	PL,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NL,	NL,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PL,	PL
};


typedef struct{
	float Kp;
	float Ki;
	float Kd;
}PID;

typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float Fp;
}CTL_PARAM;

PID fuzzy(float e,float ec);
CTL_PARAM fuzzy_ctl(float e);
// freq   gyro     dg           P   D   F
// 0.5Hz  0-200    0-40         10  1   0.4      
// 0.75Hz 100-300  0-90         20  1.4 0.2
// 1Hz    200-300  0-20,50-100  20  1.4 0.2
// 前馈   0-0.4
// 反馈P  10-20
// 反馈D  1-1.5
static const float fuzzy_rule_kp[7] = {NL,NM,NS,ZE,PS,PM,PL};
static const float fuzzy_rule_kd[7] = {NL,NM,NS,ZE,PS,PM,PL};
static const float fuzzy_rule_fp[7] = {PL,PM,PS,ZE,NS,NM,NL};


#define ROT_RATE_MAX_VALUE 400.0f
#define ROT_RATE_MID_VALUE 200.0f
#define ROT_RATE_MIN_VALUE 0.0f
#define ROT_RATE_CONVERT_COEFF 6.0f/(ROT_RATE_MAX_VALUE-ROT_RATE_MIN_VALUE)

#define KP_MAX_VALUE 20.0f
#define KP_MID_VALUE 15.0f
#define KP_MIN_VALUE 10.0f
#define KD_MAX_VALUE 1.4f
#define KD_MID_VALUE 1.2f
#define KD_MIN_VALUE 1.0f
#define FP_MAX_VALUE 0.4f
#define FP_MID_VALUE 0.2f
#define FP_MIN_VALUE 0.0f
#define KP_CONVERT_COEFF (KP_MAX_VALUE-KP_MIN_VALUE)/6.0f
#define KD_CONVERT_COEFF (KD_MAX_VALUE-KD_MIN_VALUE)/6.0f
#define FP_CONVERT_COEFF (FP_MAX_VALUE-FP_MIN_VALUE)/6.0f

/********************************************************************************
*
* 任务配置区域
*
********************************************************************************/
// 配置云台控制任务
#define FUZZY_PID_CONTROL_TASK_PRIO 6
#define FUZZY_PID_CONTROL_STK_SIZE 512
void fuzzypid_control_task(void *p_arg);

// 云台控制唤醒定时器
#define FUZZY_PID_CONTROL_TIMER_PERIOD_MS 1// 20ms的控制周期和IMU数据的周期是一致的
#define FUZZY_PID_CONTROL_TIMER_PERIOD_TICKS (((uint32_t)FUZZY_PID_CONTROL_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

void fuzzypid_control_tmrcallback();
void fuzzypid_control_task_init();





#endif
/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __PANTILT_CONTROL_H_
#define __PANTILT_CONTROL_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "includes.h"
#include "os_cfg_app.h"

#include "motor.h"

#include "rflink.h"
#include "BuffPrintf.h"
#include "robotstate.h"

//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#include "os_cfg_app.h"
#endif

#define RAD_TO_DEG 57.2975f


/********************************************************************************
*
* ������������
*
********************************************************************************/

// ������̨��������
#define PANTILT_CONTROL_TASK_PRIO 13
#define PANTILT_CONTROL_STK_SIZE 256
void pantilt_control_task(void *p_arg);

// ��̨���ƻ��Ѷ�ʱ��
#define PANTILT_CONTROL_TIMER_PERIOD_MS 10 // 20ms�Ŀ������ں�IMU���ݵ�������һ�µ�
#define PANTILT_CONTROL_TIMER_PERIOD_TICKS (((uint32_t)PANTILT_CONTROL_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

void pantilt_control_tmrcallback();
void pantilt_control_task_init();
void pantilt_control_stop();
void pantilt_control_resume();


#endif
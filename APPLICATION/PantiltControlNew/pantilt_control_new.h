/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __PANTILT_CONTROL_NEW_H_
#define __PANTILT_CONTROL_NEW_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"


#include "pwm.h"
#include "motor.h"
#include "adc.h"
#include "JY901.h"
#include "BuffPrintf.h"

//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#include "os_cfg_app.h"
#endif

#define RAD_TO_DEG 57.2975f
/********************************************************************************
*
* �ⲿ������������
*
********************************************************************************/
extern struct SIMUDATA stcIMU[2];
extern u16 g_current_adcx;
/********************************************************************************
*
* ������������
*
********************************************************************************/

// ������̨��������
#define PANTILT_CONTROL_NEW_TASK_PRIO 6
#define PANTILT_CONTROL_NEW_STK_SIZE 512
void pantilt_control_new_task(void *p_arg);

// ��̨���ƻ��Ѷ�ʱ��
#define PANTILT_CONTROL_NEW_TIMER_PERIOD_MS 5 // 20ms�Ŀ������ں�IMU���ݵ�������һ�µ�
#define PANTILT_CONTROL_NEW_TIMER_PERIOD_TICKS (((uint32_t)PANTILT_CONTROL_NEW_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

void pantilt_control_new_tmrcallback();
void pantilt_control_new_task_init();


#endif
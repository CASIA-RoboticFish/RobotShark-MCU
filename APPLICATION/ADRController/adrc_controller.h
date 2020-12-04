/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __ADRC_CONTROLLER_H_
#define __ADRC_CONTROLLER_H_

#include "math.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "motor.h"
#include "JY901.h"
#include "BuffPrintf.h"

//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#include "os_cfg_app.h"
#endif

#define PI 3.1415926
#define RAD_TO_DEG 57.2975f
/********************************************************************************
*
* �ⲿ������������
*
********************************************************************************/
extern struct SIMUDATA stcIMU[2];

/********************************************************************************
*
* ������������
*
********************************************************************************/
// ������̨��������
#define ADRC_CONTROLLER_TASK_PRIO 6
#define ADRC_CONTROLLER_STK_SIZE 512
void adrc_controller_task(void *p_arg);

// ��̨���ƻ��Ѷ�ʱ��
#define ADRC_CONTROLLER_TIMER_PERIOD_MS 10  // 20ms�Ŀ������ں�IMU���ݵ�������һ�µ�
#define ADRC_CONTROLLER_TIMER_PERIOD_TICKS (((uint32_t)ADRC_CONTROLLER_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

void adrc_controller_tmrcallback();
void adrc_controller_task_init();

#endif
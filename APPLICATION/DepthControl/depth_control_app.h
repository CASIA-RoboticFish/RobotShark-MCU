/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#ifndef _DEPTH_CONTROL_APP_H_
#define _DEPTH_CONTROL_APP_H_

// ϵͳͷ�ļ�
#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
// RFLinkͨѶ��ص�ͷ�ļ�
#include "rflink.h" 
#include "uart6_printf.h"
#include "BuffPrintf.h"
// ������״̬ͷ�ļ�
#include "robotstate.h"
// ����������ͷ�ļ�
#include "control_param.h"

// ��������
#define DEPTH_CONTROL_APP_TASK_PRIO 9  // �����������ȼ�,һ�����ٶ���256�����ȼ�������ԽС��������ȼ�Խ��
#define DEPTH_CONTROL_APP_STK_SIZE 256 // ���������ջ��С�������1������4�ֽڣ�
#define DEPTH_CONTROL_APP_TIMER_PERIOD_MS 10 // �����������ڣ�����˵�������������ÿ��10ms����һ��
#define DEPTH_CONTROL_APP_TIMER_PERIOD_TICKS (((uint32_t)DEPTH_CONTROL_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ) /* ��ʱ��ʱ��Ƶ�ʱ�����Ϊ�� 1000Hz */

// �����ʼ������
void depth_control_app_init(void);
// ������ͣ����
void depth_control_app_stop(void);
// ����ָ����к���
void depth_control_app_resume(void);

#endif
/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#ifndef _COLLISION_AVOIDANCE_APP_H_
#define _COLLISION_AVOIDANCE_APP_H_

// ϵͳͷ�ļ�
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
//������״̬
#include "robotstate.h"


// ��������
#define COLLISION_AVOIDANCE_APP_TASK_PRIO 5  // �����������ȼ�,һ�����ٶ���256�����ȼ�������ԽС��������ȼ�Խ��
#define COLLISION_AVOIDANCE_APP_STK_SIZE 256 // ���������ջ��С�������1������4�ֽڣ�
#define COLLISION_AVOIDANCE_APP_TIMER_PERIOD_MS 10 // �����������ڣ�����˵�������������ÿ��10ms����һ��
#define COLLISION_AVOIDANCE_APP_TIMER_PERIOD_TICKS (((uint32_t)COLLISION_AVOIDANCE_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ) /* ��ʱ��ʱ��Ƶ�ʱ�����Ϊ�� 1000Hz */

// �����ʼ������
void collision_avoidance_app_init(void);
// ������ͣ����
void collision_avoidance_app_stop(void);
// ����ָ����к���
void collision_avoidance_app_resume(void);

#endif
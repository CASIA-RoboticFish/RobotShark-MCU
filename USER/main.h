/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: main.h
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

// ϵͳͷ�ļ�
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "includes.h"
#include "sys.h"
#include "delay.h"

// �ײ�Ӳ��ͷ�ļ�
#include "led.h" 
#include "usart1.h"
#include "usart3.h"
#include "usart2.h"
#include "usart6.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 
#include "jy901.h"
#include "ads1115.h"
#include "ms5837.h"
#include "sdio_sdcard.h"

//#include "tail_servo_pwm.h" // V1���������V2�潫��ȥ����
//#include "pectoral_servo_pwm.h" // V1���������V2�潫��ȥ����
#include "servo_pwm.h" // V2���������ӵ�


// �м��ͷ�ļ�
#include "MyPrintf.h"
#include "BuffPrintf.h"
#include "malloc.h" 
#include "ff.h"
#include "exfuns.h"
#include "fattester.h"
#include "uart6_printf.h"

// Applicationͷ�ļ�
#include "swim_control_app.h"
#include "command_analysis_app.h"
#include "command_analysis_slave_app.h"
#include "command_analysis_friend_app.h"
#include "sensors_update_app.h"
#include "data_storage_app.h"
#include "data_sendback_app.h"
#include "data_show_app.h"
#include "target_tracking_app.h"
#include "depth_control_app.h"
#include "distributed_communicate_app.h"

// ����������ͷ�ļ�
#include "robotstate.h"

// �����
//#define MPU_EXTERNAL
//#define MPU_ONBOARD
/********************************************************************************
*
* ������������
*
********************************************************************************/
// ���ÿ�ʼ����
#define START_TASK_PRIO 3
#define START_STK_SIZE 512
OS_TCB StartTaskTCB;
// __align(8) �ǹ����ڴ����Ķ�������ϸ��������ַ
// http://www.cnblogs.com/xidongs/p/5655440.html
__align(8) CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

// ����Test����
#define TEST_TASK_PRIO 6
#define TEST_STK_SIZE 128
OS_TCB TestTaskTCB;
__align(8) CPU_STK TEST_TASK_STK[TEST_STK_SIZE];
void test_task(void *p_arg);

#endif

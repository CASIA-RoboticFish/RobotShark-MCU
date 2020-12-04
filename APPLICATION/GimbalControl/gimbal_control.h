/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef ___GIMBAL_CONTROL_H_
#define ___GIMBAL_CONTROL_H_

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
typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float Fp;
}CTL_PARAMS;

typedef struct{
	// ESO
	float wo;
	float beta01;
	float beta02;
	float beta03;
	// �Ŷ�����
	float b;
	// �����Է���
	float kp;
	float kd;
}ADRC_PARAMS;

typedef struct{
	float v1;
	float v2;
	float epsil;
	float z1;
	float z2;
	float z3;
	float z1_prev;
	float z2_prev;
	float z3_prev;
	float e1;
	float e2;
	float u;
}ADRC_VARIABLES;

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
#define ADRC_GIMBAL_CONTROL_TASK_PRIO 6
#define ADRC_GIMBAL_CONTROL_STK_SIZE 512
void adrc_gimbal_control_task(void *p_arg);

// ��̨���ƻ��Ѷ�ʱ��
#define ADRC_GIMBAL_CONTROL_TIMER_PERIOD_MS 10  // 20ms�Ŀ������ں�IMU���ݵ�������һ�µ�
#define ADRC_GIMBAL_CONTROL_TIMER_PERIOD_TICKS (((uint32_t)ADRC_GIMBAL_CONTROL_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ)

void adrc_gimbal_control_tmrcallback();
void adrc_gimbal_control_task_init();





#endif
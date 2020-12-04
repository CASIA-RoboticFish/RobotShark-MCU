/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: boxfishstate.h
Description: ���ļ�����������boxfish������״̬�Ľṹ�壬���л�����״̬�������Ϣ�ᱻ��������ṹ��
	     ������main.c�ж�����BOXFISH boxfishstate�������ļ���Ҫʹ�û�����״̬�Ļ���ֻ��Ҫ�ڴ�����ǰ�����ӣ�
	     extern BOXFISH boxfishstate; ����
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/

#ifndef _ROBOT_STATE_H_
#define _ROBOT_STATE_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


typedef enum SwimState
{
	SWIM_FORCESTOP,
	SWIM_STOP,
	SWIM_RUN,
	SWIM_INIT
}SwimState;


typedef struct SwimParam // V2�����������޸�
{
	float motion_time;
	float motion_amp;
	float motion_freq;
	float motion_offset;
	float tail_angle;
	float pecfin_angle[2];
}SwimParam;


typedef enum GimbalState
{
	GIMBAL_STOP,
	GIMBAL_ZERO,
	GIMBAL_RUN
}GimbalState;


typedef struct IMUData
{
	float roll;
	float pitch;
	float yaw;
	float accelx;
	float accely;
	float accelz;
	float gyrox;
	float gyroy;
	float gyroz;
}IMUData;

// ��������boxfishstate������״̬�Ľṹ��
typedef struct BOXFISH
{
	float timestamp; // ʱ���
	unsigned int communicate_basetime_tick; // ͨѶ��׼ʱ��
	SwimState swim_state;
	SwimParam swim_param;	
	GimbalState gimbal_state;
	IMUData onboard_imu_data;
	IMUData gimbal_imu_data;	
	uint16_t gimbal1_angle;
	uint16_t gimbal2_angle;
	float depth;
}BOXFISH;

// ��������Ŀ�����״̬�Ľṹ��
typedef struct ROBOTARGET
{
	float image_xpos;
	float image_ypos;
}ROBOTARGET;
#endif
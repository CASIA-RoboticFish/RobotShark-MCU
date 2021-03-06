/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: robosharkstate.h
Description: ���ļ�����������boxfish������״̬�Ľṹ�壬���л�����״̬�������Ϣ�ᱻ��������ṹ��
	     ������main.c�ж�����BOXFISH robosharkstate�������ļ���Ҫʹ�û�����״̬�Ļ���ֻ��Ҫ�ڴ�����ǰ�����ӣ�
	     extern BOXFISH robosharkstate; ����
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/

#ifndef _ROBOT_STATE_H_
#define _ROBOT_STATE_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

typedef enum AutoCTL
{
	AutoCTL_RUN,
	AutoCTL_STOP
}AutoCTL;

typedef enum SwimState
{
	SWIM_FORCESTOP,
	SWIM_STOP,
	SWIM_RUN,
	TAIL_STOP,
	TAIL_RUN,
	LEFT_PECTFIN_STOP,
	LEFT_PECTFIN_RUN,
	RIGHT_PECTFIN_STOP,
	RIGHT_PECTFIN_RUN,
	SWIM_INIT
}SwimState;


typedef struct SwimParam // V2�����������޸�
{
	float motion_time; // β�͵�
	float motion_amp;
	float motion_freq;
	float motion_offset;
	float tail_angle;
	float pecfin_angle[2]; // ������
	float pecfin_amp[2];
	float pecfin_freq[2];
	float pecfin_offset[2];
}SwimParam;


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

typedef struct InfraredData
{
	uint8_t obstacle_ahead;
	uint8_t obstacle_left;
	uint8_t obstacle_right;
	uint8_t obstacle_down_distance;
}InfraredData;

// ��������robosharkstate������״̬�Ľṹ��
typedef struct ROBOSHARK
{
	float timestamp; // ʱ���
	unsigned int communicate_basetime_tick; // ͨѶ��׼ʱ��
	AutoCTL autoctl_state;
	SwimState swim_state;
	SwimParam swim_param;	
	IMUData imu_data;
	InfraredData infrared_data;	
	float depth;
}ROBOSHARK;

// ��������Ŀ�����״̬�Ľṹ��
typedef struct ROBOTARGET
{
	float image_xpos;
	float image_ypos;
}ROBOTARGET;
#endif
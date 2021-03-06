/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: robosharkstate.h
Description: 本文件定义了描述boxfish机器人状态的结构体，所有机器人状态的相关信息会被存入这个结构体
	     我们在main.c中定义了BOXFISH robosharkstate，其他文件需要使用机器人状态的话，只需要在代码最前面增加：
	     extern BOXFISH robosharkstate; 即可
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


typedef struct SwimParam // V2版对其进行了修改
{
	float motion_time; // 尾巴的
	float motion_amp;
	float motion_freq;
	float motion_offset;
	float tail_angle;
	float pecfin_angle[2]; // 胸鳍的
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

// 用于描述robosharkstate机器人状态的结构体
typedef struct ROBOSHARK
{
	float timestamp; // 时间戳
	unsigned int communicate_basetime_tick; // 通讯基准时刻
	AutoCTL autoctl_state;
	SwimState swim_state;
	SwimParam swim_param;	
	IMUData imu_data;
	InfraredData infrared_data;	
	float depth;
}ROBOSHARK;

// 用于描述目标对象状态的结构体
typedef struct ROBOTARGET
{
	float image_xpos;
	float image_ypos;
}ROBOTARGET;
#endif
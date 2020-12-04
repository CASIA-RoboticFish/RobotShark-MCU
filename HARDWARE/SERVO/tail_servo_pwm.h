/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __TAIL_SERVO_PWM_H
#define __TAIL_SERVO_PWM_H

#include "stm32f4xx.h"

// 舵机通道
#define TAIL_SERVO1 0
#define TAIL_SERVO2 1
#define TAIL_SERVO3 2
#define TAIL_SERVO4 3

// 舵机PWM区间
#define SERVO_MAX_VALUE 2100
#define SERVO_MIN_VALUE 900

// 舵机中位（1号鱼）
//#define TAIL_SERVO1_MID_VALUE 1520
//#define TAIL_SERVO2_MID_VALUE 1410
//#define TAIL_SERVO3_MID_VALUE 1500
//#define TAIL_SERVO4_MID_VALUE 1500

// 舵机中位（2号鱼）
#define TAIL_SERVO1_MID_VALUE 1440
#define TAIL_SERVO2_MID_VALUE 1510
#define TAIL_SERVO3_MID_VALUE 1500
#define TAIL_SERVO4_MID_VALUE 1440


static void pwm_tim4_init ( uint32_t arr, uint32_t psc );
void tail_pwm_set_compare( uint16_t compare_value, uint8_t channel );
void tail_servo_init ( void );

#endif
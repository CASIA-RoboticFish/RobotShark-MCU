/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __SERVO_PWM_H
#define __SERVO_PWM_H

#include "stm32f4xx.h"

// 舵机通道
#define SERVO1 0 // 胸鳍1
#define SERVO2 1 // 胸鳍2
#define SERVO3 2 // 尾鳍1
#define SERVO4 3

// 舵机PWM区间
#define SERVO_MAX_VALUE 2100
#define SERVO_MIN_VALUE 900

// 舵机中位
#define SERVO1_MID_VALUE 1820
#define SERVO2_MID_VALUE 1820
#define SERVO3_MID_VALUE 1820
#define SERVO4_MID_VALUE 1520


static void pwm_tim4_init ( uint32_t arr, uint32_t psc );
void servo_pwm_set_compare( uint16_t compare_value, uint8_t channel );
void servo_init ( void );

#endif
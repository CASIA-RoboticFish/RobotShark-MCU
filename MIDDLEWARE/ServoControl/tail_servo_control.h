/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/
#ifndef __TAIL_SERVO_CONTROL_H
#define __TAIL_SERVO_CONTROL_H

#include "stm32f4xx.h"
#include "servo_pwm.h"

#define TAIL_SERVO_AMOUNT 4
#define TAIL_SERVO_1 0
#define TAIL_SERVO_2 1
#define TAIL_SERVO_3 2
#define TAIL_SERVO_4 3

#define TAIL_SERVO_1_MID_VALUE SERVO1_MID_VALUE
#define TAIL_SERVO_2_MID_VALUE SERVO2_MID_VALUE
#define TAIL_SERVO_3_MID_VALUE SERVO3_MID_VALUE
#define TAIL_SERVO_4_MID_VALUE SERVO4_MID_VALUE

#define TAIL_SERVO_MAX_VALUE SERVO_MAX_VALUE
#define TAIL_SERVO_MIN_VALUE SERVO_MIN_VALUE

void tail_servo_set_angle(float angle, uint8_t channel);

#endif
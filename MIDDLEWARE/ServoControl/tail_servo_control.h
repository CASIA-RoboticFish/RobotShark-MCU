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

#define TAIL_SERVO_AMOUNT 1
void tail_servo_set_angle(float angle, uint8_t channel);

#endif
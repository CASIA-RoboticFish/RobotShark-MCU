/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __TAIL_SERVO_CONTROL_H
#define __TAIL_SERVO_CONTROL_H

#include "stm32f4xx.h"
#include "tail_servo_pwm.h"

#define TAIL_SERVO_AMOUNT 3
void tail_servo_set_angle(float angle, uint8_t channel);

#endif
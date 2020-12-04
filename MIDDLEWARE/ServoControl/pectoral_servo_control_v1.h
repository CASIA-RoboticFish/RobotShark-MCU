/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __PECTORAL_SERVO_CONTROL_H
#define __PECTORAL_SERVO_CONTROL_H

#include "stm32f4xx.h"
#include "pectoral_servo_pwm.h"

#define PECTORAL_SERVO_AMOUNT 2
void pectoral_servo_set_angle(float angle, uint8_t channel);

#endif



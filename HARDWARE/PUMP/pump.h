/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __PUMP_H
#define __PUMP_H

#include "stm32f4xx.h"

#define PUMP_PWM_OUTPUT 800 // 这里设置了水泵默认启动的pwm大小，如果希望水泵更快，可以设得更大
#define PUMP_IN 0
#define PUMP_OUT 1
static void pwm_tim1_init(uint32_t arr, uint32_t psc);
void pump_init(void);
void pump_on(uint8_t channel);
void pump_off(uint8_t channel);
#endif
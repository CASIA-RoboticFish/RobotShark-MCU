/*****************************************************************************
Copyright: CASIA ����������ʵ����
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

#define PUMP_PWM_OUTPUT 800 // ����������ˮ��Ĭ��������pwm��С�����ϣ��ˮ�ø��죬������ø���
#define PUMP_IN 0
#define PUMP_OUT 1
static void pwm_tim1_init(uint32_t arr, uint32_t psc);
void pump_init(void);
void pump_on(uint8_t channel);
void pump_off(uint8_t channel);
#endif
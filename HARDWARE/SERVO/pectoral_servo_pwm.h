/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __PECTORAL_SERVO_PWM_H
#define __PECTORAL_SERVO_PWM_H

#include "stm32f4xx.h"

// ���ͨ��
#define PECTORAL_SERVO1 0
#define PECTORAL_SERVO2 1

// ���PWM����
#define SERVO_MAX_VALUE 2100
#define SERVO_MIN_VALUE 900

// �����λ
#define PECTORAL_SERVO1_MID_VALUE 1450
#define PECTORAL_SERVO2_MID_VALUE 1500


static void pwm_tim3_init(uint32_t arr, uint32_t psc);
void pectoral_pwm_set_compare(uint16_t compare_value, uint8_t channel);
void pectoral_servo_init();

#endif
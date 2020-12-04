/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __VALVE_H
#define __VALVE_H

#include "stm32f4xx.h"

// ��ŷ���ͨ��
#define VALVE1 1
#define VALVE2 2

void valve_init();
void valve_on(uint8_t channel);
void valve_off(uint8_t channel);

#endif
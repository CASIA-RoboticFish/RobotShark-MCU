/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"

void rw_encoder_init(void);
void rw_motor_init(void);
int rw_read_encoder(void);
void rw_motor_speed(u16 pwm);
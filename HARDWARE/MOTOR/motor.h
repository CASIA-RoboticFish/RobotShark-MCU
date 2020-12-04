/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

#define PWM_PRESCALER 84 // 此参数不可更改
#define PWM_PERIOD 1000  // PWM频率为1000Hz
#define PWM_MAX_VALUE (1000000/PWM_PERIOD) 

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3
#define MOTOR4 4

static void TB6612FNG1_init();
static void TB6612FNG2_init();
void motor_startup(uint8_t channel);
void motor_standby(uint8_t channel);
void motor_stop(uint8_t channel);
void motor_forward_rotate(uint16_t compare_value, uint8_t channel);
void motor_backward_rotate(uint16_t compare_value, uint8_t channel);
void motor_rotate(int compare_value, uint8_t channel);
void motor_init(void);



	// PE0:MOTOR1_IN1
	// PE1:MOTOR1_IN2
	// PE2:MOTOR2_IN1
	// PE3:MOTOR2_IN2
	// PD0:MOTOR12_STBY
	// PB4:MOTOR1_PWM
	// PB5:MOTOR2_PWM
	
	// PA6:MOTOR3_IN1
	// PA7:MOTOR3_IN2
	// PC4:MOTOR4_IN1
	// PC5:MOTOR4_IN2
	// PE12:MOTOR34_STBY
	// PE9:MOTOR3_PWM
	// PE11:MOTOR4_PWM
	
#endif
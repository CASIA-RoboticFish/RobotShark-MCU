/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/
#include "pectoral_servo_control.h"

static float s_f32_pectservo_angles[PECTORAL_SERVO_AMOUNT] = {0.0f, 0.0f};
static uint32_t s_u32_pectservo_center_pwms[PECTORAL_SERVO_AMOUNT] = {SERVO1_MID_VALUE, SERVO2_MID_VALUE};
static uint32_t s_u32_pectservo_current_pwms[PECTORAL_SERVO_AMOUNT] = {1500, 1500};
static float s_f32_pectservo_pwm_per_rad[PECTORAL_SERVO_AMOUNT] = {500, 500};


void pectoral_servo_set_angle(float angle, uint8_t channel)
{
	int32_t pwm_to_set = 0;
	s_f32_pectservo_angles[channel] = angle;
	pwm_to_set = s_u32_pectservo_center_pwms[channel] + (int32_t)(s_f32_pectservo_pwm_per_rad[channel]*angle);
	if(pwm_to_set < 0)
	{
		pwm_to_set = 0;
	}
	s_u32_pectservo_current_pwms[channel] = (uint32_t)pwm_to_set;
	servo_pwm_set_compare((uint32_t)pwm_to_set, channel);
}
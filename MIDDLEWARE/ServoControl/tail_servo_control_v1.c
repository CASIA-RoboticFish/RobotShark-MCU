/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "tail_servo_control.h"

static float s_f32_tailservo_angles[TAIL_SERVO_AMOUNT] = {0.0f, 0.0f, 0.0f};
static uint32_t s_u32_tailservo_center_pwms[TAIL_SERVO_AMOUNT] = {1520, 1410, 1500};
static uint32_t s_u32_tailservo_current_pwms[TAIL_SERVO_AMOUNT] = {1520, 1410, 1500};
static float s_f32_tailservo_pwm_per_rad[TAIL_SERVO_AMOUNT] = {500, 500, 500};


void tail_servo_set_angle(float angle, uint8_t channel)
{
	int32_t pwm_to_set = 0;
	s_f32_tailservo_angles[channel] = angle;
	pwm_to_set = s_u32_tailservo_center_pwms[channel] + (int32_t)(s_f32_tailservo_pwm_per_rad[channel]*angle);
	if(pwm_to_set < 0)
	{
		pwm_to_set = 0;
	}
	s_u32_tailservo_current_pwms[channel] = (uint32_t)pwm_to_set;
	tail_pwm_set_compare((uint32_t)pwm_to_set, channel);
}


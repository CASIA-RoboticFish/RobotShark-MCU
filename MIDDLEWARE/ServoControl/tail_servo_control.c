/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/
#include "tail_servo_control.h"

static float s_f32_tailservo_angles[TAIL_SERVO_AMOUNT] = {0.0f, 0.0f, 0.0f, 0.0f};
static uint32_t s_u32_tailservo_center_pwms[TAIL_SERVO_AMOUNT] = {TAIL_SERVO_1_MID_VALUE, \
								  TAIL_SERVO_2_MID_VALUE, \
								  TAIL_SERVO_3_MID_VALUE, \
                                                                  TAIL_SERVO_4_MID_VALUE};
static uint32_t s_u32_tailservo_current_pwms[TAIL_SERVO_AMOUNT] = {TAIL_SERVO_1_MID_VALUE, \
								   TAIL_SERVO_2_MID_VALUE, \
								   TAIL_SERVO_3_MID_VALUE, \
                                                                   TAIL_SERVO_4_MID_VALUE};
static float s_f32_tailservo_pwm_per_rad[TAIL_SERVO_AMOUNT] = {500, 500, 500, 500};


void tail_servo_set_angle(float angle, uint8_t channel)
{
	int32_t pwm_to_set = 0;
	s_f32_tailservo_angles[channel] = angle;
	pwm_to_set = s_u32_tailservo_center_pwms[channel] + (int32_t)(s_f32_tailservo_pwm_per_rad[channel]*angle);
	if(pwm_to_set < TAIL_SERVO_MIN_VALUE)
	{
		pwm_to_set = TAIL_SERVO_MIN_VALUE;
	}
	else if(pwm_to_set > TAIL_SERVO_MAX_VALUE)
	{
		pwm_to_set = TAIL_SERVO_MAX_VALUE;
	}
	s_u32_tailservo_current_pwms[channel] = (uint32_t)pwm_to_set;
	
	switch(channel)
	{
		case TAIL_SERVO_1:
			servo_pwm_set_compare((uint32_t)pwm_to_set, SERVO1);
			break;
		
		case TAIL_SERVO_2:
			servo_pwm_set_compare((uint32_t)pwm_to_set, SERVO2);
			break;
		
		case TAIL_SERVO_3:
			servo_pwm_set_compare((uint32_t)pwm_to_set, SERVO3);
			break;
		
		case TAIL_SERVO_4:
			servo_pwm_set_compare((uint32_t)pwm_to_set, SERVO4);
			break;
		
		default:
			break;
	}
}


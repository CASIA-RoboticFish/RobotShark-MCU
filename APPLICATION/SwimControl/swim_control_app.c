/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/
#include "swim_control_app.h"

extern BOXFISH boxfishstate;

static OS_TMR SwimControlTmr;
static OS_TCB SwimControlTCB;
__align(8) CPU_STK SWIM_CONTROL_APP_TASK_STK[SWIM_CONTROL_APP_STK_SIZE];

static float s_f32_stop_amp = 0.0f;
static float s_f32_stop_freq = 0.0f;
static float s_f32_motion_time = 0.0f;

// CPG控制
static float s_f32_cpg_state_x = 0.0001f;
static float s_f32_cpg_state_y = 0.0001f;
static float s_f32_cpg_k1 = 50;
static float s_f32_cpg_k2 = 50;
float compute_tail_cpg_model(float freq, float amp, float offset, float dt)
{
	float x = s_f32_cpg_state_x - offset;
	float y = s_f32_cpg_state_y;
	s_f32_cpg_state_x = s_f32_cpg_state_x + (-freq*y + s_f32_cpg_k1*x*(amp*amp - x*x - y*y))*dt;
	s_f32_cpg_state_y = s_f32_cpg_state_y + (freq*x + s_f32_cpg_k2*y*(amp*amp - x*x - y*y))*dt;
	return s_f32_cpg_state_x;
}
	
// 设置尾巴摆动幅度
void set_tail_amp(float amp)
{
//	if(boxfishstate.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	boxfishstate.swim_param.motion_amp = amp;
}

// 设置尾巴摆动频率
void set_tail_freq(float freq)
{
//	if(boxfishstate.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	boxfishstate.swim_param.motion_freq = freq;

}

// 设置尾巴摆动偏移
void set_tail_offset(float offset)
{
//	if(boxfishstate.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	boxfishstate.swim_param.motion_offset = offset;

}


// 设置胸鳍角度
void set_pect_fin_angle(float angle, uint8_t channel)
{
	boxfishstate.swim_param.pecfin_angle[channel] = angle;
	pectoral_servo_set_angle(angle, channel);
}



void swim_control_start(void)
{
	CPU_SR_ALLOC();
	OS_ERR err;

	if(boxfishstate.swim_state == SWIM_RUN)
	{
		return;
	}
	else if(boxfishstate.swim_state == SWIM_STOP)
	{
		set_tail_amp(s_f32_stop_amp);
		set_tail_freq(s_f32_stop_freq);
		s_f32_cpg_k1 = 50;
	  s_f32_cpg_k2 = 50;
	}
	else if(boxfishstate.swim_state == SWIM_FORCESTOP)
	{
		set_tail_amp(s_f32_stop_amp);
		set_tail_freq(s_f32_stop_freq);
		s_f32_cpg_k1 = 50;
	  s_f32_cpg_k2 = 50;
		s_f32_motion_time = 0.0f;
		s_f32_cpg_state_x = 0.0001f;
		s_f32_cpg_state_y = 0.0001f;
		OS_CRITICAL_ENTER();
		OSTaskResume(&SwimControlTCB,
								 &err);
		OSTmrStart(&SwimControlTmr,
							 &err);
		OS_CRITICAL_EXIT();
	}
	else if(boxfishstate.swim_state == SWIM_INIT)
	{
		// Do nothing!
	}

	boxfishstate.swim_state = SWIM_RUN;
}


void swim_control_stop(void)
{
	if(boxfishstate.swim_state != SWIM_RUN)
	{
			return;
	}
	s_f32_stop_amp = boxfishstate.swim_param.motion_amp;
	set_tail_amp(0.0f);
	s_f32_stop_freq = boxfishstate.swim_param.motion_freq;
	set_tail_freq(0.0f);
	s_f32_cpg_k1 = 0;
	s_f32_cpg_k2 = 0;
	boxfishstate.swim_state = SWIM_STOP;
}


void swim_control_forcestop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	int i = 0;

	if(boxfishstate.swim_state == SWIM_FORCESTOP)
	{
			return;
	}

	OS_CRITICAL_ENTER();

	OSTmrStop(&SwimControlTmr,
						OS_OPT_TMR_NONE,
						0,
						&err);

	OSTaskSuspend(&SwimControlTCB,
								&err);

	OS_CRITICAL_EXIT();
	
	s_f32_stop_amp = boxfishstate.swim_param.motion_amp;
	set_tail_amp(0.0f);
	s_f32_stop_freq = boxfishstate.swim_param.motion_freq;
	set_tail_freq(0.0f);
	s_f32_cpg_k1 = 0;
	s_f32_cpg_k2 = 0;
	
	tail_servo_set_angle(0, 0);
	boxfishstate.swim_state = SWIM_FORCESTOP;
}



static void swim_control_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&SwimControlTCB,
								OS_OPT_POST_NONE,
								&err);
}



static void swim_control_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;

	int i = 0;

	OS_TICK now_os_tick;
	OS_TICK last_os_tick;
	float time_interval;
	float tail_angle = 0.0f;
	float amp = boxfishstate.swim_param.motion_amp;
	float freq = boxfishstate.swim_param.motion_freq;
	float offset = boxfishstate.swim_param.motion_offset;

	
	set_tail_amp(boxfishstate.swim_param.motion_amp);
	set_tail_freq(boxfishstate.swim_param.motion_freq);
	set_tail_offset(boxfishstate.swim_param.motion_offset);

	
	now_os_tick = OSTimeGet(&err);
	last_os_tick = now_os_tick;

	while(1)
	{
			OSTaskSemPend(0,
										OS_OPT_PEND_BLOCKING,
										&ts,
										&err);

			now_os_tick = OSTimeGet(&err);
			time_interval = (float)(now_os_tick - last_os_tick) / (float)(OS_CFG_TICK_RATE_HZ);
			last_os_tick = now_os_tick;

			s_f32_motion_time += time_interval*boxfishstate.swim_param.motion_freq;	
			boxfishstate.swim_param.motion_time = s_f32_motion_time;
			amp = boxfishstate.swim_param.motion_amp/180*3.1415926; // 单位是弧度
			freq = boxfishstate.swim_param.motion_freq*2*3.1415926; // 单位是角频率，2pi代表1Hz 
			offset = boxfishstate.swim_param.motion_offset/180*3.1415926; // 单位是弧度
			//tail_angle = amp * sin(freq*s_f32_motion_time) + offset; // 简单的正弦运动的控制律，可以进行修改
			tail_angle = compute_tail_cpg_model(freq, amp, offset, time_interval);
			tail_servo_set_angle(tail_angle, 0);
		
			// 机器人状态，记录尾鳍摆动的角度
			boxfishstate.swim_param.tail_angle = tail_angle;

	}
}


void swim_control_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&SwimControlTCB,
							 "Swim Control App",
							 swim_control_app_task,
							 0,
							 SWIM_CONTROL_APP_TASK_PRIO,
							 &SWIM_CONTROL_APP_TASK_STK[0],
							 SWIM_CONTROL_APP_STK_SIZE/10,
							 SWIM_CONTROL_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&SwimControlTmr,
							"Swim Control Timer",
							0,
							SWIM_CONTROL_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)swim_control_app_tmrcallback,
							0,
							&err);
	
	swim_control_forcestop();

	OS_CRITICAL_EXIT();
}
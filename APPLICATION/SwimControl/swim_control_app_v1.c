/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "swim_control_app.h"

extern ROBOREMORA roboremora;

static OS_TMR SwimControlTmr;
static OS_TCB SwimControlTCB;
__align(8) CPU_STK SWIM_CONTROL_APP_TASK_STK[SWIM_CONTROL_APP_STK_SIZE];

static struct CPGModel s_s_cpgmodel;
static float s_f32_stop_amp = 0.0f;
static float s_f32_cpg_time = 0.0f;

// 设置尾巴摆动幅度
void set_tail_amp(float amp)
{
//	if(roboremora.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	roboremora.swim_param.cpg_amp = amp;
	CPG_SetAmplitudeRate(&s_s_cpgmodel, amp);
}

// 设置尾巴摆动频率
void set_tail_freq(float freq)
{
//	if(roboremora.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	roboremora.swim_param.cpg_freq = freq;
	CPG_SetFrequency(&s_s_cpgmodel, freq);
}

// 设置尾巴摆动偏移
void set_tail_offset(float offset)
{
//	if(roboremora.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	roboremora.swim_param.cpg_offset = offset;
	CPG_SetOffsetRate(&s_s_cpgmodel, offset);
}

// 设置尾巴摆动相位差
void set_tail_phaselag(float phase_lag)
{
//	if(roboremora.swim_state != SWIM_RUN)
//	{
//			return;
//	}
	roboremora.swim_param.phase_lag = phase_lag;
  CPG_SetPhaseDiff(&s_s_cpgmodel, phase_lag);
}



void set_pect_fin_angle(float angle, uint8_t channel)
{
	roboremora.swim_param.pecfin_angle[channel] = angle;
	pectoral_servo_set_angle(angle, channel);
}



void swim_control_start(void)
{
	CPU_SR_ALLOC();
	OS_ERR err;

	if(roboremora.swim_state == SWIM_RUN)
	{
		return;
	}
	else if(roboremora.swim_state == SWIM_STOP)
	{
		set_tail_amp(s_f32_stop_amp);
	}
	else if(roboremora.swim_state == SWIM_FORCESTOP)
	{
		OS_CRITICAL_ENTER();
		CPG_Reset(&s_s_cpgmodel);
		s_f32_cpg_time = 0.0f;
		OSTaskResume(&SwimControlTCB,
								 &err);
		OSTmrStart(&SwimControlTmr,
							 &err);
		OS_CRITICAL_EXIT();
	}
	else if(roboremora.swim_state == SWIM_INIT)
	{
		// Do nothing!
	}

	roboremora.swim_state = SWIM_RUN;
}


void swim_control_stop(void)
{
	if(roboremora.swim_state != SWIM_RUN)
	{
			return;
	}
	s_f32_stop_amp = roboremora.swim_param.cpg_amp;
	set_tail_amp(0.0f);
	roboremora.swim_state = SWIM_STOP;
}


void swim_control_forcestop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	int i = 0;

	if(roboremora.swim_state == SWIM_FORCESTOP)
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
	
	
	for(i = 0; i < s_s_cpgmodel.node_num; i++)
	{
			tail_servo_set_angle(0, i);
	}
	
	roboremora.swim_state = SWIM_FORCESTOP;
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

	CPG_InitModelDefault(&s_s_cpgmodel, TAIL_SERVO_AMOUNT);

	set_tail_amp(roboremora.swim_param.cpg_amp);
	set_tail_freq(roboremora.swim_param.cpg_freq);
	set_tail_offset(roboremora.swim_param.cpg_offset);

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
					
			CPG_UpdateModel(&s_s_cpgmodel, time_interval);
		
			s_f32_cpg_time += time_interval*roboremora.swim_param.cpg_freq;
			if(s_f32_cpg_time>1)
				s_f32_cpg_time -= 1;
			
			roboremora.swim_param.cpg_time = s_f32_cpg_time;
			
			for(i = 0; i < s_s_cpgmodel.node_num; i++)
			{
				tail_servo_set_angle(s_s_cpgmodel.nodes[i].angle, i);
				// 机器人状态，记录尾鳍摆动的角度
				roboremora.swim_param.tail_angle[i] = s_s_cpgmodel.nodes[i].angle;
				//	if(i==2) tail_servo_set_angle(0, i);					
				//BuffPrintf("%f\n",s_s_cpgmodel.nodes[i].angle);
			}

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
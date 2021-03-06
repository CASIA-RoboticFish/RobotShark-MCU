/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#include "target_tracking_app.h"
extern ROBOSHARK robosharkstate;

static OS_TMR TargetTrackingTmr;
static OS_TCB TargetTrackingTCB;
__align(8) CPU_STK TARGET_TRACKING_APP_TASK_STK[TARGET_TRACKING_APP_STK_SIZE];


static void target_tracking_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&TargetTrackingTCB,
								OS_OPT_POST_NONE,
								&err);
}

static void target_tracking_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK now_os_tick;

	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
	}
}

void target_tracking_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&TargetTrackingTCB,
							 "Target Tracking App",
							 target_tracking_app_task,
							 0,
							 TARGET_TRACKING_APP_TASK_PRIO,
							 &TARGET_TRACKING_APP_TASK_STK[0],
							 TARGET_TRACKING_APP_STK_SIZE/10,
							 TARGET_TRACKING_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&TargetTrackingTmr,
							"Target Tracking Timer",
							0,
							TARGET_TRACKING_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)target_tracking_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&TargetTrackingTmr,
						&err);
	
	target_tracking_app_stop();
							 
	OS_CRITICAL_EXIT();
}

// 暂停运行
void target_tracking_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&TargetTrackingTmr,
		OS_OPT_TMR_NONE,
		0,
		&err);

	OSTaskSuspend(&TargetTrackingTCB, &err);

	OS_CRITICAL_EXIT();
}

// 恢复运行
void target_tracking_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&TargetTrackingTCB, &err);

	OSTmrStart(&TargetTrackingTmr, &err);

	OS_CRITICAL_EXIT();
}
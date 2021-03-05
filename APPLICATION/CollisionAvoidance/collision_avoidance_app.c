/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/
#include "collision_avoidance_app.h"

extern BOXFISH boxfishstate;
static OS_TMR CollisionAvoidanceTmr; // 定义一个定时器
static OS_TCB CollisionAvoidanceTCB; // 定义一个任务控制块
__align(8) CPU_STK COLLISION_AVOIDANCE_APP_TASK_STK[COLLISION_AVOIDANCE_APP_STK_SIZE]; // 定义任务堆栈

/****************************************************************************
该函数实现一个简单的避障功能
输入：前方 / 下方 / 左侧 / 右侧 的测距返回值
控制条件：在自动游动模式下执行自主避障 // 手动控制下关闭
输出：避障控制律
*****************************************************************************/
static void collision_avoidance_app_task(void * p_arg)
{
    OS_ERR err;
    CPU_TS ts;
    
    int i = 0;
    OS_TICK now_os_tick;
    OS_TICK last_os_tick;
    
    while( 1) 
    {
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);
        now_os_tick = OSTimeGet(&err);
        boxfishstate.timestamp = (float)now_os_tick/(float)(OS_CFG_TICK_RATE_HZ);

    }
}

void collision_avoidance_app_tmrcallback()
{
    OS_ERR err;
    
    OSTaskSemPost(&CollisionAvoidanceTCB, OS_OPT_POST_NONE,&err);
}


void collision_avoidance_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&CollisionAvoidanceTCB,
							 "Collision Avoidance Task",
							 collision_avoidance_app_task,
							 0,
							 COLLISION_AVOIDANCE_APP_TASK_PRIO,
							 &COLLISION_AVOIDANCE_APP_TASK_STK[0],
							 COLLISION_AVOIDANCE_APP_STK_SIZE/10,
							 COLLISION_AVOIDANCE_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&CollisionAvoidanceTmr,
							"Collision Avoidance Timer",
							0,
							COLLISION_AVOIDANCE_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)collision_avoidance_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&CollisionAvoidanceTmr,
						&err);
							 
	OS_CRITICAL_EXIT();
}
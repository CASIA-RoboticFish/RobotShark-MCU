/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#include "depth_control_app.h"

extern BOXFISH boxfishstate;

static OS_TMR DepthControlTmr; // 定义一个定时器
static OS_TCB DepthControlTCB; // 定义一个任务控制块
__align(8) CPU_STK DEPTH_CONTROL_APP_TASK_STK[DEPTH_CONTROL_APP_STK_SIZE]; // 定义任务堆栈

// 定时器回调函数，每当计时到达，就会执行这个函数，这个函数给任务发送信号量，从而唤醒任务
static void depth_control_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&DepthControlTCB, // 发送信号量给DepthControl任务
			OS_OPT_POST_NONE,
			&err);
}

// 任务主函数，就是会被反复执行的函数
static void depth_control_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	uint8_t databuf[12];
	
	while(1)
	{
		// 等待信号量，每当信号量到来，就继续往下执行，否则就进入挂起的状态
		OSTaskSemPend(0,
			      OS_OPT_PEND_BLOCKING,
			      &ts,
			      &err);
		
		// 任务主体代码
		// 这里由大家自由发挥
		databuf[0] = 0xAA;
		databuf[1] = 0xBB;
		databuf[2] = 0xCC;
		databuf[3] = 0xDD;
		databuf[4] = 0xEE;
		databuf[5] = 0xFF;
		RFLink_sendStruct(BOXFISH_5, SET_SWIM_RUN , databuf , 0);
	}
}

// 任务初始化函数
void depth_control_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();
	
	// 创建任务
	OSTaskCreate(&DepthControlTCB, 		// 任务控制块
		 "Depth Control App", 		// 任务名称
		 depth_control_app_task, 	// 任务主函数
		 0,				// 传递给任务的参数
		 DEPTH_CONTROL_APP_TASK_PRIO,   // 任务优先级
		 &DEPTH_CONTROL_APP_TASK_STK[0], // 任务堆栈基地址
		 DEPTH_CONTROL_APP_STK_SIZE/10,  // 任务堆栈深度限位
		 DEPTH_CONTROL_APP_STK_SIZE,     // 任务堆栈大小
		 0, // 任务任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
		 0, // 当使能时间片轮转时的时间片长度，为 0 时为默认长度
		 0, // 用户补充的存储区
		 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, // 任务的特定选项
							  // OS_OPT_TASK_NONE      表示没有任何选项 
							  // OS_OPT_TASK_STK_CHK   指定是否允许检测该任务的堆栈 
							  // OS_OPT_TASK_STK_CLR   指定是否清除该任务的堆栈 
							  // OS_OPT_TASK_SA VE_FP  指定是否存储浮点寄存器，CPU需要有浮点运算硬件并且有专用代码保存浮点寄存器。 
		 &err);
	
	// 创建定时器
	OSTmrCreate(&DepthControlTmr,    // 定时器指针
		"Depth Control Timer",   // 定时器名字
		0,                       // 初始化定时器的延迟值
		DEPTH_CONTROL_APP_TIMER_PERIOD_TICKS, // 重复周期
		OS_OPT_TMR_PERIODIC, // 定时器运行选项, OS_OPT_TMR_ONE_SHOT代表单次定时器；OS_OPT_TMR_PERIODIC代表周期定时器 
		(OS_TMR_CALLBACK_PTR)depth_control_app_tmrcallback, // 定时器回调函数
		0, // 回调函数参数
		&err);

	OSTmrStart(&DepthControlTmr, &err); // 启动定时器
	
	depth_control_app_stop();
							 
	OS_CRITICAL_EXIT();
}

// 暂停运行
void depth_control_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&DepthControlTmr, // 暂停定时器
		OS_OPT_TMR_NONE,
		0,
		&err);

	OSTaskSuspend(&DepthControlTCB, &err); // 任务挂起

	OS_CRITICAL_EXIT();
}

// 恢复运行
void depth_control_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&DepthControlTCB, &err); // 重启定时器

	OSTmrStart(&DepthControlTmr, &err); // 启动任务

	OS_CRITICAL_EXIT();
}
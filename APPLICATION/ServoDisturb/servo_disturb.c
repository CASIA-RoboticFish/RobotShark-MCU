/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "servo_disturb.h"

// UCOS相关变量定义
OS_TCB RollServoDisturbTaskTCB;
__align(8) CPU_STK ROLL_SERVO_DISTURB_TASK_STK[ROLL_SERVO_DISTURB_STK_SIZE];
static OS_TMR RollServoTmr;

OS_TCB YawServoDisturbTaskTCB;
__align(8) CPU_STK YAW_SERVO_DISTURB_TASK_STK[YAW_SERVO_DISTURB_STK_SIZE];
static OS_TMR YawServoTmr;


static void roll_servo_tmrcallBack ( void )
{
    OS_ERR err;

    OSTaskSemPost(&RollServoDisturbTaskTCB,
                  OS_OPT_POST_NONE,
                  &err);    
}	

static void yaw_servo_tmrcallBack ( void )
{
    OS_ERR err;

    OSTaskSemPost(&YawServoDisturbTaskTCB,
                  OS_OPT_POST_NONE,
                  &err);    
}	

// 任务主函数
void roll_servo_disturb_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK tick;
	float t = 0.0f;
	
	SERVO_CH servo_channel;
	servo_channel = SERVO_1;
	float pi = 3.1415926;
	float angle_interval = 0.0;
	float servo1_rotate_amplitude = 40;    // 单位：度
	float servo1_rotate_speed = 3.1667;    // 单位：度每毫秒
	float servo1_expect_frequency = 0.5;     // 单位：Hz
	float servo1_current_time = 0;
	float servo1_current_angle = 0;
	uint32_t servo1_time_interval = 1;
	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		tick = OSTimeGet(&err);
    t = (float)tick / (float)OS_CFG_TICK_RATE_HZ;
		servo1_current_angle = servo1_rotate_amplitude*sin(2*pi*servo1_expect_frequency*t);
		pwm_set_angle(servo1_current_angle,servo_channel);
		
	}
}

// 任务主函数
void yaw_servo_disturb_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK tick;
	float t = 0.0f;
	
	SERVO_CH servo_channel;
	servo_channel = SERVO_2;
	float pi = 3.1415926;
	float angle_interval = 0.0;
	//float servo2_rotate_amplitude = 40;    // 单位：度 //大概60度
	float servo2_rotate_amplitude = 22;
	float servo2_rotate_speed = 3.1667;    // 单位：度每毫秒
	float servo2_expect_frequency = 0.5;     // 单位：Hz
	float servo2_current_time = 0;
	float servo2_current_angle = 0;
	uint32_t servo2_time_interval = 10;
	
	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		tick = OSTimeGet(&err);
    t = (float)tick / (float)OS_CFG_TICK_RATE_HZ;
		servo2_current_angle = servo2_rotate_amplitude*sin(2*pi*servo2_expect_frequency*t);
		pwm_set_angle(servo2_current_angle,servo_channel);
	}
}


// 创建扰动舵机运转任务
void roll_servo_disturb_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&RollServoDisturbTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"roll servo disturb control task",         //任务名字
							 (OS_TASK_PTR )roll_servo_disturb_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )ROLL_SERVO_DISTURB_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&ROLL_SERVO_DISTURB_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)ROLL_SERVO_DISTURB_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)ROLL_SERVO_DISTURB_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值

	OSTmrCreate(&RollServoTmr,
							"roll control timer",
							0,
							ROLL_SERVO_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)roll_servo_tmrcallBack,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&RollServoTmr,
             &err);
							 
	OS_CRITICAL_EXIT();
}

// 创建扰动舵机运转任务
void yaw_servo_disturb_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&YawServoDisturbTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"yaw servo disturb control task",         //任务名字
							 (OS_TASK_PTR )yaw_servo_disturb_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )YAW_SERVO_DISTURB_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&YAW_SERVO_DISTURB_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)YAW_SERVO_DISTURB_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)YAW_SERVO_DISTURB_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值

	 OSTmrCreate(&YawServoTmr,
							"gimbal control timer",
							0,
							YAW_SERVO_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)yaw_servo_tmrcallBack,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&YawServoTmr,
             &err);
							 
	OS_CRITICAL_EXIT();
}
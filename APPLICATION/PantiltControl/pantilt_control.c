/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "pantilt_control.h"
extern ROBOSHARK robosharkstate;

// UCOS相关变量定义
OS_TCB PantiltControlTaskTCB;
__align(8) CPU_STK PANTILT_CONTROL_TASK_STK[PANTILT_CONTROL_STK_SIZE];
OS_TMR PantiltControlTaskTmr; // 软件定时器


// 云台角度
static float yaw_target_angle = 0.0f;
static float feedforward_target_angle = 0.0f;


void pantilt_control_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&PantiltControlTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}


void pantilt_control_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	OS_TICK os_tick_now;
	OS_TICK last_os_tick;
	
	os_tick_now = OSTimeGet(&err);
	last_os_tick = os_tick_now;
	
	float yaw_angle_now = 0.0f;
	float yaw_angle_last = 0.0f;
	float d_angle = 0.0f;
	float gyro_z_now = 0.0f;
 	float input_err = 0.0f;
	float integral_err = 0.0f;
	float dt = 0.0f;
	
	float output_p = 0.0f;
	float output_i = 0.0f;
	float output_d = 0.0f;
	float output = 3000.0f;

	
	
	float p = 0.0f;
	float i = 0.0f;
  float d = 0.0f;
	p = 10;
	i = 0;
	d = 1;

	float feedforward_angle = 0.0f;
	float feedforward_gyroz = 0.0f;
	float feedforward_output = 0.0f;
	float last_feedforward_gyroz[2] = {0.0f};
	float feedforward_d_gyroz = 0.0f;
  float last_feedforward_d_gyroz = 0.0f;
	
	int pwm = 0;
	u16 print_cnt = 0;
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		os_tick_now = OSTimeGet(&err);
		dt = (float)(os_tick_now - last_os_tick) / (float)OS_CFG_TICK_RATE_HZ;
		last_os_tick = os_tick_now;
		
		yaw_angle_now = robosharkstate.imu_data.yaw;
		gyro_z_now = robosharkstate.imu_data.gyroz;

		d_angle = (yaw_angle_now-yaw_angle_last)/dt;
		yaw_angle_last = yaw_angle_now;
		
		input_err = yaw_target_angle - yaw_angle_now;
		integral_err = integral_err + input_err * dt;
		
		
		/* P Control Part */
		if(input_err < (2.0f / RAD_TO_DEG) && input_err > (-2.0f / RAD_TO_DEG))
		{
				input_err = 0;
				integral_err = 0;
		}
		output_p = input_err * p;
		
		/* I Control Part */
		output_i = integral_err * i;
		
		
		/* D Control Part */
		if(gyro_z_now < 0.001f && gyro_z_now > -0.001f)
		{
				gyro_z_now = 0;
		}
		output_d = - d_angle * d;

		
		/* Sum the PD Control together */
		output = output_p + output_i + output_d;
		

		//feedforward_gyroz = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
		/* The angle scale here is deg */

		//feedforward_output = feedforward_gyroz * 0.2f;
		
//		feedforward_d_gyroz = (feedforward_gyroz - last_feedforward_gyroz[0]) * 1.0f;
//		feedforward_output += feedforward_d_gyroz * 0.1175f;
//		feedforward_output += (feedforward_d_gyroz - last_feedforward_d_gyroz) * 1.0f * 0.0001407414f;
//		feedforward_output = 3*feedforward_output;
		
		//output = output - feedforward_output;
		
//		last_feedforward_d_gyroz = feedforward_d_gyroz;
//    last_feedforward_gyroz[1] = last_feedforward_gyroz[0];
//    last_feedforward_gyroz[0] = feedforward_gyroz;
//		

		// 前馈环节
		pwm = (int) output;
		//motor_rotate(pwm, MOTOR3);

		
		print_cnt ++;
		if (print_cnt == 10)
		{
			print_cnt = 0;
			//BuffPrintf("%f, %d\n", yaw_angle_now, pwm);
		}
		
		
	}
}


// 创建云台控制任务
void pantilt_control_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
		
	OSTaskCreate((OS_TCB    * )&PantiltControlTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"pan tilt control task",         //任务名字
							 (OS_TASK_PTR )pantilt_control_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )PANTILT_CONTROL_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&PANTILT_CONTROL_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)PANTILT_CONTROL_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)PANTILT_CONTROL_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&PantiltControlTaskTmr,
							"pan tilt control timer",
							0,
							PANTILT_CONTROL_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)pantilt_control_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&PantiltControlTaskTmr,
             &err);
	
	pantilt_control_stop();
							 
	OS_CRITICAL_EXIT();
}

// 暂停运行
void pantilt_control_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&PantiltControlTaskTmr,
						OS_OPT_TMR_NONE,
						0,
						&err);

	OSTaskSuspend(&PantiltControlTaskTCB,
								&err);

	OS_CRITICAL_EXIT();
}

// 恢复运行
void pantilt_control_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&PantiltControlTaskTCB,
						 &err);

	OSTmrStart(&PantiltControlTaskTmr,
					 &err);

	OS_CRITICAL_EXIT();
	
	yaw_target_angle = robosharkstate.imu_data.yaw;
	
}

/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "pantilt_control_new.h"

// UCOS相关变量定义
OS_TCB PantiltControlNewTaskTCB;
__align(8) CPU_STK PANTILT_CONTROL_NEW_TASK_STK[PANTILT_CONTROL_NEW_STK_SIZE];
OS_TMR PantiltControlNewTaskTmr; // 软件定时器


// 云台角度
static float roll_target_angle = 0.0f;
static float yaw_target_angle = 0.0f;

void pantilt_control_new_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&PantiltControlNewTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}

void pantilt_control_new_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	OS_TICK os_tick_now;
	OS_TICK last_os_tick;
	os_tick_now = OSTimeGet(&err);
	last_os_tick = os_tick_now;
	
	float dt = 0.0f;

  
	float position_p = 10;
	float position_i = 0.0f;
	float position_d = 1.0f;
	
	float angle_last = 0.0f;
	float angle_curr = 0.0f;
	float gyro_curr = 0.0f;
	
	float angle_err = 0.0f;
	float integral_err = 0.0f;
	
	float velocity_p = 0.1;
	float velocity_i = 0.00f;
	float velocity_d = 0.01f;

	
	u16 encoder_last = g_current_adcx;
	u16 encoder_curr = g_current_adcx;
	int vel_feedback = 0;
	float vel_target = 0.0f;
	float vel_err = 0.0f;
	float last_vel_err = 0.0f;
	float last_last_vel_err = 0.0f;
	float vel_output = 0.0f;
	
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
		
		angle_curr = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		gyro_curr = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
		angle_err = yaw_target_angle - angle_curr;
		integral_err += angle_err * dt;
		
		/* P Control Part */
		if(angle_err < (1.0f / RAD_TO_DEG) && angle_err > (-1.0f / RAD_TO_DEG))
		{
			angle_err = 0;
			integral_err = 0;
		}
		
		/* D Control Part */
		if(gyro_curr < 0.001f && gyro_curr > -0.001f)
		{
			gyro_curr = 0;
		}

		/* Sum the PD Control together */
		vel_target = angle_err * position_p + integral_err * position_i - gyro_curr * position_d;
		
		
		
		encoder_curr = g_current_adcx;
		vel_feedback = encoder_curr - encoder_last;
		encoder_last = encoder_curr;
		
		vel_err = vel_target - vel_feedback;
		if ((vel_err < 5)&&(vel_err > -5))
		{
			vel_err = 0;
		}
		//
		
		vel_output += velocity_p * (vel_err - last_vel_err) + velocity_i * vel_err \
									+ velocity_d*(vel_err - 2*last_vel_err + last_last_vel_err);
		
		
		feedforward_gyroz = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
		/* The angle scale here is deg */
		feedforward_output = feedforward_gyroz * 0.1f;
		feedforward_d_gyroz = (feedforward_gyroz - last_feedforward_gyroz[0]) * 1.0f;
		feedforward_output += feedforward_d_gyroz * 0.1175f;
		feedforward_output += (feedforward_d_gyroz - last_feedforward_d_gyroz) * 1.0f * 0.0001407414f;
		feedforward_output = 0.01*feedforward_output;
		vel_output = vel_output - feedforward_output;
		
		last_feedforward_d_gyroz = feedforward_d_gyroz;
    last_feedforward_gyroz[1] = last_feedforward_gyroz[0];
    last_feedforward_gyroz[0] = feedforward_gyroz;
		
		pwm = (int) vel_output;

		motor_rotate(pwm, MOTOR_2);
		last_last_vel_err = last_vel_err;
		last_vel_err = vel_err;
		
		print_cnt++;
		if(print_cnt==50)
		{
			BuffPrintf("AE: %f!\n", angle_err);
			BuffPrintf("VE: %f!\n", vel_err);
			BuffPrintf("VE1: %f!\n", vel_target);
			BuffPrintf("VE2: %d!\n", vel_feedback);
			BuffPrintf("P: %d!\n", pwm);
			print_cnt = 0;
		}
	}
}

// 创建云台控制任务
void pantilt_control_new_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	int count = 0;
	while(count < 40)
	{
		roll_target_angle = roll_target_angle + (float)stcIMU[1].stcAngle.Angle[0]/32768*180;
		yaw_target_angle = yaw_target_angle + (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		delay_ms(50);
		count++;		
	}
	roll_target_angle = roll_target_angle / count;
	yaw_target_angle = yaw_target_angle / count;
	
	OSTaskCreate((OS_TCB    * )&PantiltControlNewTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"pan tilt control task",         //任务名字
							 (OS_TASK_PTR )pantilt_control_new_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )PANTILT_CONTROL_NEW_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&PANTILT_CONTROL_NEW_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)PANTILT_CONTROL_NEW_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)PANTILT_CONTROL_NEW_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&PantiltControlNewTaskTmr,
							"pan tilt control timer",
							0,
							PANTILT_CONTROL_NEW_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)pantilt_control_new_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&PantiltControlNewTaskTmr,
             &err);
	
							 
	OS_CRITICAL_EXIT();
}



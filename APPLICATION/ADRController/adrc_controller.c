/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "adrc_controller.h"

// UCOS相关变量定义
OS_TCB AdrcControllerTaskTCB;
__align(8) CPU_STK ADRC_CONTROLLERL_TASK_STK[ADRC_CONTROLLER_STK_SIZE];
OS_TMR AdrcControllerTaskTmr; // 软件定时器

// 云台角度
static float yaw_target_angle = 0.0f;




void adrc_controller_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	OS_TICK os_tick_now;
	OS_TICK last_os_tick;
	os_tick_now = OSTimeGet(&err);
	last_os_tick = os_tick_now;
	float dt = 0.0f;
	uint16_t print_cnt = 0;
	
	// ADRC 参数
	// ESO
	float wo = 70.0f; // observer frequency
	float beta01 = 3*wo;
	float beta02 = 3*wo*wo;
	float beta03 = wo*wo*wo;
	// 扰动补偿
	float b = 15.0f;
	// 非线性反馈
	float wc = 10.0f; // controller frequency
	float kp = wc*wc;
	float kd = 2*wc;
	kp = 20.0f;
	kd = 0.0f;

	
	// 辅助变量
	// 安排过渡过程
	float v1 = yaw_target_angle;
	float v2 = 0.0f;
	// ESO
	float epsil = 0.0f;
	float z1 = 0.0f;
	float z2 = 0.0f;
	float z3 = 0.0f;
	float z1_prev = 0.0f;
	float z2_prev = 0.0f;
	float z3_prev = 0.0f;
	// 控制量形成
	float e1 = 0.0f;
	float e2 = 0.0f;
	float u = 0.0f;
	
	
	// 传感器反馈
	float angle = 0.0f;
	float gyro = 0.0f;
	float lastangle = 0.0f;
	float dangle = 0.0f;
	// 电机控制
	int pwm = 0;
	
	// 启动时间
	int t = 0;
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		os_tick_now = OSTimeGet(&err);
		dt = (float)(os_tick_now - last_os_tick) / (float)OS_CFG_TICK_RATE_HZ;
		last_os_tick = os_tick_now;
		
		angle = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		gyro = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
		
		// ESO
		epsil = z1_prev - angle;
	  z1 = z1_prev + dt*(z2_prev-beta01*epsil);
		z2 = z2_prev + dt*(z3_prev-beta02*epsil+b*u);
		z3 = z3_prev + dt*(-beta03*epsil);
		z1_prev = z1;
		z2_prev = z2;
		z3_prev = z3;
		
		// 控制量形成
		e1 = v1 - z1;
		e2 = v2 - z2;
		u= kp*e1 - kd*z2 - z3/b;
		//u= kp*e1 - kd*z2;
		dangle = (lastangle - angle)/dt;
		lastangle = angle;
		//u= (kp*(v1-angle) + kd*dangle);
		
		//u= kp*e1 - kd*gyro - z3/b;
		
		if((v1-angle) < (1.0f / RAD_TO_DEG) && (v1-angle) > (-1.0f / RAD_TO_DEG))
		{
				u = 0;
		}
		
		
		pwm = (int)u;
		motor_rotate(pwm, MOTOR_2);
		
		
		print_cnt++;
		if(print_cnt==1)
		{
			//BuffPrintf("ADRC\n");
			//BuffPrintf("%f,%f,%f,%f;\n",angle,z1,u,z3/b);
			//BuffPrintf("%f,%f,%f,%f;\n",dangle,gyro,z2,u);
			//BuffPrintf("%f,%f;\n",dangle,z2);
			//BuffPrintf("%f,%f;\n",angle,z1);
			//BuffPrintf("%f,%f;\n",u,z3/b);
			BuffPrintf("%f\n",angle);
			print_cnt = 0;
		}
		
	}
}


void adrc_controller_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&AdrcControllerTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}


void adrc_controller_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	int count = 0;
	while(count < 40)
	{
		yaw_target_angle = yaw_target_angle + (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		delay_ms(50);
		count++;		
	}
	
	yaw_target_angle = yaw_target_angle / count;

	
	OSTaskCreate((OS_TCB    * )&AdrcControllerTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"ardc control task",         //任务名字
							 (OS_TASK_PTR )adrc_controller_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )ADRC_CONTROLLER_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&ADRC_CONTROLLERL_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)ADRC_CONTROLLER_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)ADRC_CONTROLLER_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&AdrcControllerTaskTmr,
							"adrc control timer",
							0,
							ADRC_CONTROLLER_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)adrc_controller_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&AdrcControllerTaskTmr,
             &err);
	
							 
	OS_CRITICAL_EXIT();
}

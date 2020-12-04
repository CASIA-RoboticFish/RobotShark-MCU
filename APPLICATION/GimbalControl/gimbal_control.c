/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "gimbal_control.h"

// UCOS相关变量定义
OS_TCB ADRCGimbalControlTaskTCB;
__align(8) CPU_STK ADRC_GIMBAL_CONTROL_TASK_STK[ADRC_GIMBAL_CONTROL_STK_SIZE];
OS_TMR ADRCGimbalControlTaskTmr; // 软件定时器

// 云台角度
static float roll_target_angle = 0.0f;
static float yaw_target_angle = 0.0f;

//void adrc_gimbal_control_task(void *p_arg)
//{
//	OS_ERR err;
//	CPU_TS ts;
//	
//	OS_TICK os_tick_now;
//	OS_TICK last_os_tick;
//	os_tick_now = OSTimeGet(&err);
//	last_os_tick = os_tick_now;
//	float dt = 0.0f;
//	uint16_t print_cnt = 0;
//	
//	// ADRC 参数
//	// ESO
//	float wo = 20.0f; // observer frequency
//	float beta01 = 3*wo;
//	float beta02 = 3*wo*wo;
//	float beta03 = wo*wo*wo;
//	// 扰动补偿
//	float b = 12.5f;
//	// 非线性反馈
//	float wc = 10.0f; // controller frequency
//	float kp = wc*wc;
//	float kd = 2*wc;
//	kp = 200.0f;
//	kd = 5.0f;

//		
//	// 辅助变量
//	// 安排过渡过程
//	float v1 = roll_target_angle;
//	float v2 = 0.0f;
//	// ESO
//	float epsil = 0.0f;
//	float z1 = 0.0f;
//	float z2 = 0.0f;
//	float z3 = 0.0f;
//	float z1_prev = 0.0f;
//	float z2_prev = 0.0f;
//	float z3_prev = 0.0f;
//	// 控制量形成
//	float e1 = 0.0f;
//	float e2 = 0.0f;
//	float u = 0.0f;
//	
//	
//	// 传感器反馈
//	float angle = 0.0f;
//	float gyro = 0.0f;
//	float lastangle = 0.0f;
//	float dangle = 0.0f;
//	// 电机控制
//	int pwm = 0;
//	
//	// 启动时间
//	int t = 0;
//	while(1)
//	{
//		OSTaskSemPend(0,
//									OS_OPT_PEND_BLOCKING,
//									&ts,
//									&err);
//		
//		os_tick_now = OSTimeGet(&err);
//		dt = (float)(os_tick_now - last_os_tick) / (float)OS_CFG_TICK_RATE_HZ;
//		last_os_tick = os_tick_now;
//		
//		angle = (float)stcIMU[1].stcAngle.Angle[0]/32768*180;
//		gyro = (float)stcIMU[1].stcGyro.w[0]/32768*2000;
//		
//		// ESO
//		epsil = z1_prev - angle;
//	  z1 = z1_prev + dt*(z2_prev-beta01*epsil);
//		z2 = z2_prev + dt*(z3_prev-beta02*epsil+b*u);
//		z3 = z3_prev + dt*(-beta03*epsil);
//		z1_prev = z1;
//		z2_prev = z2;
//		z3_prev = z3;
//		
//		// 控制量形成
//		e1 = v1 - z1;
//		e2 = v2 - z2;
//		u= kp*e1 - kd*z2 - z3/b;
//		//u= kp*e1 - kd*z2;
//		dangle = (lastangle - angle)/dt;
//		lastangle = angle;
//		//u= (kp*(v1-angle) + kd*dangle);
//		
//		//u= kp*e1 - kd*gyro - z3/b;
//		
//		if((v1-angle) < (1.0f / RAD_TO_DEG) && (v1-angle) > (-1.0f / RAD_TO_DEG))
//		{
//				u = 0;
//		}
//		
//		
//		pwm = (int)u;
//		motor_rotate(pwm, MOTOR_1);
//		
//		
//		print_cnt++;
//		if(print_cnt==1)
//		{
//			//BuffPrintf("ADRC\n");
//			//BuffPrintf("%f,%f,%f,%f;\n",angle,z1,u,z3/b);
//			//BuffPrintf("%f,%f,%f,%f;\n",dangle,gyro,z2,u);
//			//BuffPrintf("%f,%f;\n",dangle,z2);
//			//BuffPrintf("%f,%f;\n",angle,z1);
//			//BuffPrintf("%f,%f;\n",u,z3/b);
//			BuffPrintf("%f\n",angle);
//			print_cnt = 0;
//		}
//		
//	}
//}

//void adrc_gimbal_control_task(void *p_arg)
//{
//	OS_ERR err;
//	CPU_TS ts;
//	
//	OS_TICK os_tick_now;
//	OS_TICK last_os_tick;
//	os_tick_now = OSTimeGet(&err);
//	last_os_tick = os_tick_now;
//	float dt = 0.0f;
//	uint16_t print_cnt = 0;
//	
//	// ADRC 参数
//	// ESO
//	float wo = 20.0f; // observer frequency
//	float beta01 = 3*wo;
//	float beta02 = 3*wo*wo;
//	float beta03 = wo*wo*wo;
//	// 扰动补偿
//	float b = 10.0f;
//	// 非线性反馈
//	float wc = 10.0f; // controller frequency
//	float kp = wc*wc;
//	float kd = 2*wc;
//	kp = 200.0f;
//	kd = 5.0f;

//		
//	// 辅助变量
//	// 安排过渡过程
//	float v1 = yaw_target_angle;
//	float v2 = 0.0f;
//	// ESO
//	float epsil = 0.0f;
//	float z1 = 0.0f;
//	float z2 = 0.0f;
//	float z3 = 0.0f;
//	float z1_prev = 0.0f;
//	float z2_prev = 0.0f;
//	float z3_prev = 0.0f;
//	// 控制量形成
//	float e1 = 0.0f;
//	float e2 = 0.0f;
//	float u = 0.0f;
//	
//	
//	// 传感器反馈
//	float angle = 0.0f;
//	float gyro = 0.0f;
//	float lastangle = 0.0f;
//	float dangle = 0.0f;
//	// 电机控制
//	int pwm = 0;
//	
//	// 启动时间
//	int t = 0;
//	while(1)
//	{
//		OSTaskSemPend(0,
//									OS_OPT_PEND_BLOCKING,
//									&ts,
//									&err);
//		
//		os_tick_now = OSTimeGet(&err);
//		dt = (float)(os_tick_now - last_os_tick) / (float)OS_CFG_TICK_RATE_HZ;
//		last_os_tick = os_tick_now;
//		
//		angle = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
//		gyro = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
//		
//		// ESO
//		epsil = z1_prev - angle;
//	  z1 = z1_prev + dt*(z2_prev-beta01*epsil);
//		z2 = z2_prev + dt*(z3_prev-beta02*epsil+b*u);
//		z3 = z3_prev + dt*(-beta03*epsil);
//		z1_prev = z1;
//		z2_prev = z2;
//		z3_prev = z3;
//		
//		// 控制量形成
//		e1 = v1 - z1;
//		e2 = v2 - z2;
//		u= kp*e1 - kd*z2 - z3/b;
//		//u= kp*e1 - kd*z2;
//		dangle = (lastangle - angle)/dt;
//		lastangle = angle;
//		//u= (kp*(v1-angle) + kd*dangle);
//		
//		//u= kp*e1 - kd*gyro - z3/b;
//		
//		if((v1-angle) < (1.0f / RAD_TO_DEG) && (v1-angle) > (-1.0f / RAD_TO_DEG))
//		{
//				u = 0;
//		}
//		
//		
//		pwm = (int)u;
//		motor_rotate(pwm, MOTOR_2);
//		
//		
//		print_cnt++;
//		if(print_cnt==1)
//		{
//			//BuffPrintf("ADRC\n");
//			//BuffPrintf("%f,%f,%f,%f;\n",angle,z1,u,z3/b);
//			//BuffPrintf("%f,%f,%f,%f;\n",dangle,gyro,z2,u);
//			//BuffPrintf("%f,%f;\n",dangle,z2);
//			//BuffPrintf("%f,%f;\n",angle,z1);
//			//BuffPrintf("%f,%f;\n",u,z3/b);
//			BuffPrintf("%f\n",angle);
//			print_cnt = 0;
//		}
//		
//	}
//}

void adrc_gimbal_control_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	OS_TICK os_tick_now;
	OS_TICK last_os_tick;
	os_tick_now = OSTimeGet(&err);
	last_os_tick = os_tick_now;
	float dt = 0.0f;
	
	float psi_c = 0.0f;
	float theta_c = 0.0f;
	float phi_c = 0.0f;
	float alpha_z = 0.0f;
	float alpha_x = 0.0f;
	
	float cos_psi_c = 0.0f;
	float cos_theta_c = 0.0f;
	float cos_phi_c = 0.0f;
	float sin_psi_c = 0.0f;
	float sin_theta_c = 0.0f;
	float sin_phi_c = 0.0f;
	
	// Roll Params
	float roll_angle = 0.0f;
	float gyro_x = 0.0f;

	ADRC_PARAMS roll_adrc_param;
	roll_adrc_param.wo = 20.0f;
	roll_adrc_param.beta01 = 3*roll_adrc_param.wo;
	roll_adrc_param.beta02 = 3*roll_adrc_param.wo*roll_adrc_param.wo;
	roll_adrc_param.beta03 = roll_adrc_param.wo*roll_adrc_param.wo*roll_adrc_param.wo;
	roll_adrc_param.b = 12.5f;
	roll_adrc_param.kp = 250.0f;
	roll_adrc_param.kd = 5.0f;
	
	ADRC_VARIABLES roll_adrc_var = {roll_target_angle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, \
																	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	int roll_pwm = 0;
	
	// Yaw Params
	float yaw_angle = 0.0f;
	float gyro_z = 0.0f;

	ADRC_PARAMS yaw_adrc_param;
	yaw_adrc_param.wo = 20.0f;
	yaw_adrc_param.beta01 = 3*yaw_adrc_param.wo;
	yaw_adrc_param.beta02 = 3*yaw_adrc_param.wo*yaw_adrc_param.wo;
	yaw_adrc_param.beta03 = yaw_adrc_param.wo*yaw_adrc_param.wo*yaw_adrc_param.wo;
	yaw_adrc_param.b = 10.0f;
	yaw_adrc_param.kp = 200.0f;
	yaw_adrc_param.kd = 5.0f;
	
	ADRC_VARIABLES yaw_adrc_var = {yaw_target_angle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, \
																	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	int yaw_pwm = 0;
																	
	u16 print_cnt = 0;
	float psi = 0.0f;
	float theta = 0.0f;
	float phi = 0.0f;
																	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		os_tick_now = OSTimeGet(&err);
		dt = (float)(os_tick_now - last_os_tick) / (float)OS_CFG_TICK_RATE_HZ;
		last_os_tick = os_tick_now;
		
		psi_c = (float)stcIMU[1].stcAngle.Angle[2]/32768 * PI;
		theta_c = (float)stcIMU[1].stcAngle.Angle[1]/32768 * PI;
		phi_c = (float)stcIMU[1].stcAngle.Angle[0]/32768 * PI;
		
		cos_psi_c = cos(psi_c);
		sin_psi_c = sin(psi_c);
		cos_theta_c = cos(theta_c);
		sin_theta_c = sin(theta_c);
		cos_phi_c = cos(phi_c);
		sin_phi_c = sin(phi_c);
		alpha_z = atan((cos_psi_c*sin_theta_c*sin_phi_c-sin_psi_c*cos_phi_c)/cos_psi_c/cos_theta_c)*180/PI;
		alpha_x = atan((sin_psi_c*sin_theta_c*cos_phi_c-cos_psi_c*sin_phi_c)/cos_phi_c/cos_theta_c)*180/PI;
		
		// Roll Channel
    roll_angle = (float)stcIMU[1].stcAngle.Angle[0]/32768*180;
    gyro_x = (float)stcIMU[1].stcGyro.w[0]/32768*2000;
		
		// ESO
		roll_adrc_var.epsil = roll_adrc_var.z1_prev - roll_angle;
	  roll_adrc_var.z1 = roll_adrc_var.z1_prev + dt*(roll_adrc_var.z2_prev-roll_adrc_param.beta01*roll_adrc_var.epsil);
		roll_adrc_var.z2 = roll_adrc_var.z2_prev + dt*(roll_adrc_var.z3_prev-roll_adrc_param.beta02*roll_adrc_var.epsil+roll_adrc_param.b*roll_adrc_var.u);
		roll_adrc_var.z3 = roll_adrc_var.z3_prev + dt*(-roll_adrc_param.beta03*roll_adrc_var.epsil);
		roll_adrc_var.z1_prev = roll_adrc_var.z1;
		roll_adrc_var.z2_prev = roll_adrc_var.z2;
		roll_adrc_var.z3_prev = roll_adrc_var.z3;
		
		// 控制量形成
		roll_adrc_var.e1 = roll_adrc_var.v1 - roll_adrc_var.z1;
		roll_adrc_var.e2 = roll_adrc_var.v2 - roll_adrc_var.z2;
		roll_adrc_var.u= roll_adrc_param.kp*roll_adrc_var.e1 - roll_adrc_param.kd*roll_adrc_var.z2 - roll_adrc_var.z3/roll_adrc_param.b;

		if((roll_adrc_var.v1-roll_angle) < (1.0f / RAD_TO_DEG) && (roll_adrc_var.v1-roll_angle) > (-1.0f / RAD_TO_DEG))
		{
				roll_adrc_var.u = 0;
		}
		
		roll_pwm = (int)roll_adrc_var.u;
		motor_rotate(roll_pwm, MOTOR_1);

		
		// Roll Channel
    yaw_angle = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
    gyro_z = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
		
		// ESO
		yaw_adrc_var.epsil = yaw_adrc_var.z1_prev - yaw_angle;
	  yaw_adrc_var.z1 = yaw_adrc_var.z1_prev + dt*(yaw_adrc_var.z2_prev-yaw_adrc_param.beta01*yaw_adrc_var.epsil);
		yaw_adrc_var.z2 = yaw_adrc_var.z2_prev + dt*(yaw_adrc_var.z3_prev-yaw_adrc_param.beta02*yaw_adrc_var.epsil+yaw_adrc_param.b*yaw_adrc_var.u);
		yaw_adrc_var.z3 = yaw_adrc_var.z3_prev + dt*(-yaw_adrc_param.beta03*yaw_adrc_var.epsil);
		yaw_adrc_var.z1_prev = yaw_adrc_var.z1;
		yaw_adrc_var.z2_prev = yaw_adrc_var.z2;
		yaw_adrc_var.z3_prev = yaw_adrc_var.z3;
		
		// 控制量形成
		yaw_adrc_var.e1 = yaw_adrc_var.v1 - yaw_adrc_var.z1;
		yaw_adrc_var.e2 = yaw_adrc_var.v2 - yaw_adrc_var.z2;
		yaw_adrc_var.u= yaw_adrc_param.kp*yaw_adrc_var.e1 - yaw_adrc_param.kd*yaw_adrc_var.z2 - yaw_adrc_var.z3/yaw_adrc_param.b;

		if((yaw_adrc_var.v1-yaw_angle) < (1.0f / RAD_TO_DEG) && (yaw_adrc_var.v1-yaw_angle) > (-1.0f / RAD_TO_DEG))
		{
				yaw_adrc_var.u = 0;
		}
		
		yaw_pwm = (int)yaw_adrc_var.u;
		motor_rotate(yaw_pwm, MOTOR_2);
		
		print_cnt++;
		if(print_cnt==5)
		{
//			phi = (float)stcIMU[0].stcAngle.Angle[0]/32768*180;
//			theta = (float)stcIMU[0].stcAngle.Angle[1]/32768*180;
//			psi = (float)stcIMU[0].stcAngle.Angle[2]/32768*180;
			BuffPrintf("%f,%f,%f;\n",phi_c,theta_c,psi_c);
			print_cnt = 0;
		}
		
	}
}


void adrc_gimbal_control_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&ADRCGimbalControlTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}


void adrc_gimbal_control_task_init()
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

	
	OSTaskCreate((OS_TCB    * )&ADRCGimbalControlTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"adrc gimbal control task",         //任务名字
							 (OS_TASK_PTR )adrc_gimbal_control_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )ADRC_GIMBAL_CONTROL_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&ADRC_GIMBAL_CONTROL_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)ADRC_GIMBAL_CONTROL_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)ADRC_GIMBAL_CONTROL_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&ADRCGimbalControlTaskTmr,
							"adrc gimbal control timer",
							0,
							ADRC_GIMBAL_CONTROL_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)adrc_gimbal_control_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&ADRCGimbalControlTaskTmr,
             &err);
	
							 
	OS_CRITICAL_EXIT();
}

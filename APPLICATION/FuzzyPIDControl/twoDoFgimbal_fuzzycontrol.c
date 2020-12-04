/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "twoDoFgimbal_fuzzycontrol.h"

// UCOS相关变量定义
OS_TCB GimbalControlTaskTCB;
__align(8) CPU_STK GIMBAL_CONTROL_TASK_STK[GIMBAL_CONTROL_STK_SIZE];
OS_TMR GimbalControlTaskTmr; // 软件定时器

// 云台角度
static float roll_target_angle = 0.0f;
static float yaw_target_angle = 0.0f;



CTL_PARAM gimbal_fuzzy_ctl(float e)
{
	 
	 float etemp,ectemp;
	 float eLefttemp,ecLefttemp;
	 float eRighttemp ,ecRighttemp;

	 int eLeftIndex,ecLeftIndex;
	 int eRightIndex,ecRightIndex;
	 CTL_PARAM ctl_param;
	 float kp = 0.0f;
	 float kd = 0.0f;
	 float fp = 0.0f;
	 etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));
	 
	 eLeftIndex = (int)e;
   eRightIndex = eLeftIndex;
   eLeftIndex = (int)((etemp-0.5) + 3);        //[-3,3] -> [0,6]
   eRightIndex = (int)((etemp+0.5) + 3);
	 
   eLefttemp =etemp == 0.0 ? 0.0:((etemp+0.5)-e);
   eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-0.5));
	 
//	 BuffPrintf("el:%d, \n", eLeftIndex); 
//	 BuffPrintf("er:%d, \n", eRightIndex); 
	
// 
//	/*************************************反模糊*************************************/
   
	kp = eLefttemp*fuzzy_rule_kp[eLeftIndex] + eRighttemp*fuzzy_rule_kp[eRightIndex];
	kd = eLefttemp*fuzzy_rule_kd[eLeftIndex] + eRighttemp*fuzzy_rule_kd[eRightIndex];
	fp = eLefttemp*fuzzy_rule_fp[eLeftIndex] + eRighttemp*fuzzy_rule_fp[eRightIndex];
		
	ctl_param.Kp = kp*KP_CONVERT_COEFF + KP_MID_VALUE;
	ctl_param.Kd = kd*KD_CONVERT_COEFF + KD_MID_VALUE;
	ctl_param.Fp = fp*FP_CONVERT_COEFF + FP_MID_VALUE;
	//ctl_param.Fp = 0.4;
	BuffPrintf("kp:%f, kd:%f, fp:%f \n", ctl_param.Kp, ctl_param.Kd, ctl_param.Fp);
	return ctl_param;
}


void gimbal_control_task(void *p_arg)
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
	float roll_angle_now = 0.0f;
	float gyro_x_now = 0.0f;
 	float roll_input_err = 0.0f;
	float roll_integral_err = 0.0f;
	
	float roll_output_p = 0.0f;
	float roll_output_i = 0.0f;
	float roll_output_d = 0.0f;
	float roll_output = 0.0f;

	CTL_PARAM roll_ctl_param = {50.0f, 0.0f, 2.0f, 1.0f};
	
	float roll_feedforward_gyroz = 0.0f;
	float roll_feedforward_output = 0.0f;

	int roll_pwm = 0;
	
	// Yaw Params
	float yaw_angle_now = 0.0f;
	float gyro_z_now = 0.0f;
 	float yaw_input_err = 0.0f;
	float yaw_integral_err = 0.0f;
	
	float yaw_output_p = 0.0f;
	float yaw_output_i = 0.0f;
	float yaw_output_d = 0.0f;
	float yaw_output = 0.0f;
	float yaw_output_last = 0.0f;
	// 0.50Hz // 50 // 2.0 // 1.0
	// 0.75Hz // 70 // 4.0 // 2.0
	// 
	//CTL_PARAM yaw_ctl_param = {20.0f, 0.0f, 1.4f, 0.4f};
  //CTL_PARAM yaw_ctl_param = {10.0f, 0.0f, 1.0f, 0.4f};
	//CTL_PARAM yaw_ctl_param = {30.0f, 0.0f, 1.4f, 0.4f};
	CTL_PARAM yaw_ctl_param = {55.0f, 0.0f, 2.0f, 1.0f};
	float yaw_feedforward_gyroz = 0.0f;
	float yaw_feedforward_output = 0.0f;
	float yaw_feedforward_gyroz_last = 0.0f;
	float yaw_feedforward_gyroz_err = 0.0f;
	float yaw_feedforward_gyroz_err_last = 0.0f;
	float yaw_feedforward_gyroz_err_err = 0.0f;
	int yaw_pwm = 0;
	
	float fuzzy_input = 0.0f;
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
		
		/*******************************************************************************/
		/*******************************************************************************/
		// Roll通道
		roll_angle_now = (float)stcIMU[1].stcAngle.Angle[0]/32768*180;
		gyro_x_now = (float)stcIMU[1].stcGyro.w[0]/32768*2000;
		
		roll_input_err = roll_target_angle - roll_angle_now;
		roll_input_err = alpha_x;
		roll_integral_err = roll_integral_err + roll_input_err * dt;
		
		
		/* P Control Part */
		if(roll_input_err < (1.0f / RAD_TO_DEG) && roll_input_err > (-1.0f / RAD_TO_DEG))
		{
				roll_input_err = 0;
				roll_integral_err = 0;
		}
		roll_output_p = roll_input_err * roll_ctl_param.Kp;
		
		/* I Control Part */
		roll_output_i = roll_integral_err * roll_ctl_param.Ki;
		
		
		/* D Control Part */
		if(gyro_x_now < 0.001f && gyro_x_now > -0.001f)
		{
				gyro_x_now = 0;
		}
		roll_output_d = -(gyro_x_now) * roll_ctl_param.Kd;

		
		/* Sum the PD Control together */
		roll_output = roll_output_p + roll_output_i + roll_output_d;
		
		/* FeedForward value */
		roll_feedforward_gyroz = (float)stcIMU[0].stcGyro.w[0]/32768*2000;
		roll_feedforward_output = roll_feedforward_gyroz * roll_ctl_param.Fp;
		
		/* Sum the FB & FF Control together */
		roll_output = roll_output - roll_feedforward_output;
		

		// 前馈环节
		roll_pwm = (int) roll_output;
		motor_rotate(roll_pwm, MOTOR_1);
		
		/*******************************************************************************/
		/*******************************************************************************/
		// Yaw 通道
		yaw_angle_now = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		gyro_z_now = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
		
		yaw_input_err = yaw_target_angle - yaw_angle_now;
		//yaw_input_err = alpha_z;
		yaw_integral_err = yaw_integral_err + yaw_input_err * dt;
		
		
		/* P Control Part */
		if(yaw_input_err < (1.0f / RAD_TO_DEG) && yaw_input_err > (-1.0f / RAD_TO_DEG))
		{
				yaw_input_err = 0;
				yaw_integral_err = 0;
		}
		yaw_output_p = yaw_input_err * yaw_ctl_param.Kp;
		
		/* I Control Part */
		yaw_output_i = yaw_integral_err * yaw_ctl_param.Ki;
		
		
		/* D Control Part */
		if(gyro_z_now < 0.001f && gyro_z_now > -0.001f)
		{
				gyro_z_now = 0;
		}
		yaw_output_d = -(gyro_z_now) * yaw_ctl_param.Kd;

		
		/* Sum the PD Control together */
		yaw_output = yaw_output_p + yaw_output_i + yaw_output_d;
		
		/* FeedForward value */
		yaw_feedforward_gyroz = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
		
		//yaw_feedforward_gyroz_err = (yaw_feedforward_gyroz - yaw_feedforward_gyroz_last);
		//yaw_feedforward_gyroz_err_err = (yaw_feedforward_gyroz_err - yaw_feedforward_gyroz_err_last);
		
		yaw_feedforward_output = yaw_feedforward_gyroz * yaw_ctl_param.Fp;
		//yaw_feedforward_output += yaw_feedforward_gyroz_err*3;
		//yaw_feedforward_output += yaw_feedforward_gyroz_err_err*3;
		
		//yaw_feedforward_gyroz_last = yaw_feedforward_gyroz;
		//yaw_feedforward_gyroz_err_last = yaw_feedforward_gyroz_err;

		
		/* Sum the FB & FF Control together */
		yaw_output = yaw_output - yaw_feedforward_output;
		
		//yaw_output = (yaw_output + yaw_output_last)/2;
		//yaw_output_last = yaw_output;
		
		// 前馈环节
		yaw_pwm = (int) yaw_output;
		motor_rotate(yaw_pwm, MOTOR_2);
		
		
		print_cnt++;
		if(print_cnt==50)
		{
			yaw_angle_now = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
			//delta_gyro_z = gyro_z_now - gyro_z_last;
			//gyro_z_last = gyro_z_now;
			//BuffPrintf("%f,%f;\n",yaw_angle_now,yaw_feedforward_gyroz);
			BuffPrintf("%f\n",yaw_feedforward_output);
			//BuffPrintf("yer%f\n",yaw_feedforward_gyroz_err);
			//BuffPrintf("yerer%f\n",yaw_feedforward_gyroz_err_err);
			//BuffPrintf("Gimbal Control\n");
			//BuffPrintf("pwm:%d\n",yaw_pwm);
			//BuffPrintf("yfo:%f\n",yaw_feedforward_output);
			//BuffPrintf("yo:%f\n", yaw_output);
			//BuffPrintf("%f\n",yaw_feedforward_gyroz_err);
			//BuffPrintf("dg:%f, \n", delta_gyro_z);
			//BuffPrintf("e:%f, \n", e);
			//BuffPrintf("az:%f, ez:%f\n", alpha_z, yaw_input_err);
			//BuffPrintf("ax:%f, ex:%f\n", alpha_x, roll_input_err);
			
//			fuzzy_input = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
//			if(fuzzy_input<0)
//				fuzzy_input = -fuzzy_input;
//			else
//				fuzzy_input = fuzzy_input;
			
//			if(abs(fuzzy_input)<ROT_RATE_MIN_VALUE)
//				fuzzy_input = ROT_RATE_MIN_VALUE;
//			if(abs(fuzzy_input)>ROT_RATE_MAX_VALUE)
//				fuzzy_input = ROT_RATE_MAX_VALUE;
		
			//yaw_ctl_param = gimbal_fuzzy_ctl(ROT_RATE_CONVERT_COEFF*(fuzzy_input-ROT_RATE_MID_VALUE));
			print_cnt = 0;
		}
		
	}
}


void gimbal_control_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&GimbalControlTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}


void gimbal_control_task_init()
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

	
	OSTaskCreate((OS_TCB    * )&GimbalControlTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"gimbal control task",         //任务名字
							 (OS_TASK_PTR )gimbal_control_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )GIMBAL_CONTROL_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&GIMBAL_CONTROL_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)GIMBAL_CONTROL_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)GIMBAL_CONTROL_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&GimbalControlTaskTmr,
							"gimbal control timer",
							0,
							GIMBAL_CONTROL_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)gimbal_control_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&GimbalControlTaskTmr,
             &err);
	
							 
	OS_CRITICAL_EXIT();
}

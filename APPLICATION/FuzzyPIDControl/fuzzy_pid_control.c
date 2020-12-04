/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "fuzzy_pid_control.h"

// UCOS相关变量定义
OS_TCB FuzzyPIDControlTaskTCB;
__align(8) CPU_STK FUZZY_PID_CONTROL_TASK_STK[FUZZY_PID_CONTROL_STK_SIZE];
OS_TMR FuzzyPIDControlTaskTmr; // 软件定时器

// 云台角度
static float roll_target_angle = 0.0f;
static float yaw_target_angle = 0.0f;


//PID fuzzy(float e,float ec)
//{
//	 float etemp,ectemp;
//	 float eLefttemp,ecLefttemp;
//	 float eRighttemp ,ecRighttemp;

//	 int eLeftIndex,ecLeftIndex;
//	 int eRightIndex,ecRightIndex;
//	 PID      fuzzy_PID;
//	 etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));

//	 eLeftIndex = (int)e;
//	 eRightIndex = eLeftIndex;
//	 eLeftIndex = (int)((etemp-0.5) + 3);        //[-3,3] -> [0,6]
//	 eRightIndex = (int)((etemp+0.5) + 3);

//	 eLefttemp =etemp == 0.0 ? 0.0:((etemp+0.5)-e);
//	 eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-0.5));

//	 ectemp = ec > 3.0 ? 0.0 : (ec < - 3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5: (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0) ))));

//	 ecLeftIndex = (int)((ectemp-0.5) + 3);        //[-3,3] -> [0,6]
//	 ecRightIndex = (int)((ectemp+0.5) + 3);

//	 ecLefttemp =ectemp == 0.0 ? 0.0:((ectemp+0.5)-ec);
//	 ecRighttemp=ectemp == 0.0 ? 0.0:( ec-(ectemp-0.5));
// 
//	/*************************************反模糊*************************************/
// 
//	fuzzy_PID.Kp = (eLefttemp * ecLefttemp *  fuzzyRuleKp[ecLeftIndex][eLeftIndex]                    
//					+ eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex]
//					+ eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex]
//					+ eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]);
// 
//	fuzzy_PID.Ki =   (eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex]
//					+ eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex]
//					+ eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex]
//					+ eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]);
// 
//	fuzzy_PID.Kd = (eLefttemp * ecLefttemp *    fuzzyRuleKd[ecLeftIndex][eLeftIndex]
//					+ eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex]
//					+ eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex]
//					+ eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]);
//					
//	return fuzzy_PID;
//}

CTL_PARAM fuzzy_ctl(float e)
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
	BuffPrintf("kp:%f, kd:%f, fp:%f \n", ctl_param.Kp, ctl_param.Kd, ctl_param.Fp);
	return ctl_param;
}

void fuzzypid_control_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	OS_TICK os_tick_now;
	OS_TICK last_os_tick;
	os_tick_now = OSTimeGet(&err);
	last_os_tick = os_tick_now;
	float dt = 0.0f;
	
	float yaw_angle_now = 0.0f;
	float gyro_z_now = 0.0f;
 	float input_err = 0.0f;
	float integral_err = 0.0f;
	
	float output_p = 0.0f;
	float output_i = 0.0f;
	float output_d = 0.0f;
	float output = 0.0f;

	CTL_PARAM ctl_param = {10.0f, 0.0f, 1.0f, 0.2f};
	
	float feedforward_gyroz = 0.0f;
	float feedforward_output = 0.0f;

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
		
		yaw_angle_now = (float)stcIMU[1].stcAngle.Angle[2]/32768*180;
		gyro_z_now = (float)stcIMU[1].stcGyro.w[2]/32768*2000;
		
		input_err = yaw_target_angle - yaw_angle_now;
		integral_err = integral_err + input_err * dt;
		
		
		/* P Control Part */
		if(input_err < (1.0f / RAD_TO_DEG) && input_err > (-1.0f / RAD_TO_DEG))
		{
				input_err = 0;
				integral_err = 0;
		}
		output_p = input_err * ctl_param.Kp;
		
		/* I Control Part */
		output_i = integral_err * ctl_param.Ki;
		
		
		/* D Control Part */
		if(gyro_z_now < 0.001f && gyro_z_now > -0.001f)
		{
				gyro_z_now = 0;
		}
		output_d = -(gyro_z_now) * ctl_param.Kd;

		
		/* Sum the PD Control together */
		output = output_p + output_i + output_d;
		
		/* FeedForward value */
		feedforward_gyroz = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
		feedforward_output = feedforward_gyroz * ctl_param.Fp;
		
		/* Sum the FB & FF Control together */
		output = output - feedforward_output;
		

		// 前馈环节
		pwm = (int) output;
		motor_rotate(pwm, MOTOR_2);
		
		
		print_cnt++;
		if(print_cnt==25)
		{
			yaw_angle_now = (float)stcIMU[0].stcAngle.Angle[2]/32768*180;
			gyro_z_now = (float)stcIMU[0].stcGyro.w[2]/32768*2000;
			//delta_gyro_z = gyro_z_now - gyro_z_last;
			//gyro_z_last = gyro_z_now;
			
			BuffPrintf("gz:%f, \n", gyro_z_now);
			//BuffPrintf("dg:%f, \n", delta_gyro_z);
			//BuffPrintf("e:%f, \n", e);
			
			ctl_param = fuzzy_ctl(ROT_RATE_CONVERT_COEFF*(abs(gyro_z_now)-ROT_RATE_MID_VALUE));
			print_cnt = 0;
		}
		
	}
}


void fuzzypid_control_tmrcallback()
{
	OS_ERR err;
	OSTaskSemPost(&FuzzyPIDControlTaskTCB,
								OS_OPT_POST_NONE,
								&err);
}


void fuzzypid_control_task_init()
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

	
	OSTaskCreate((OS_TCB    * )&FuzzyPIDControlTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"fuzzy pid control task",         //任务名字
							 (OS_TASK_PTR )fuzzypid_control_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )FUZZY_PID_CONTROL_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&FUZZY_PID_CONTROL_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)FUZZY_PID_CONTROL_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)FUZZY_PID_CONTROL_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OSTmrCreate(&FuzzyPIDControlTaskTmr,
							"fuzzy pid control timer",
							0,
							FUZZY_PID_CONTROL_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)fuzzypid_control_tmrcallback,
							0,
							&err);
	
	// 开启定时器
	OSTmrStart(&FuzzyPIDControlTaskTmr,
             &err);
	
							 
	OS_CRITICAL_EXIT();
}

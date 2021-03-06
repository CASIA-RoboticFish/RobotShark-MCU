/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "data_show_app.h"
extern ROBOSHARK robosharkstate;

static OS_TMR DataShowTmr;
static OS_TCB DataShowTCB;
__align(8) CPU_STK DATA_SHOW_APP_TASK_STK[DATA_SHOW_APP_STK_SIZE];

static Command s_datatype; // 数据类型
static uint8_t s_dataaxis; // 数据轴向（X，Y，Z）

void set_data_type(Command command, float data)
{
	s_datatype = command;
	s_dataaxis = (uint8_t)data;
}

static void data_show_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&DataShowTCB,
								OS_OPT_POST_NONE,
								&err);
}

static void data_show_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	
	uint8_t databuf[12];
	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		
		switch(s_datatype)
		{
			case READ_IMU1_ATTITUDE:				
				switch(s_dataaxis)
				{
					case 1:
						memcpy(databuf, &(robosharkstate.imu_data.roll),  4);
						break;
					case 2:
						memcpy(databuf, &(robosharkstate.imu_data.pitch),  4);
						break;
					case 3:
						memcpy(databuf, &(robosharkstate.imu_data.yaw),  4);
						break;
				}
				break;
			
			case READ_IMU1_ACCEL:
				switch(s_dataaxis)
				{
					case 1:
						memcpy(databuf, &(robosharkstate.imu_data.accelx), 4);
						break;
					case 2:
						memcpy(databuf, &(robosharkstate.imu_data.accely), 4);
						break;
					case 3:
						memcpy(databuf, &(robosharkstate.imu_data.accelz), 4);
						break;
				}
				break;
			
			case READ_IMU1_GYRO:
				switch(s_dataaxis)
				{
					case 1:
						memcpy(databuf, &(robosharkstate.imu_data.gyrox), 4);
						break;
					case 2:
						memcpy(databuf, &(robosharkstate.imu_data.gyroy), 4);
						break;
					case 3:
						memcpy(databuf, &(robosharkstate.imu_data.gyroz), 4);
						break;
				}
				break;
			
			
			case READ_DEPTH:
				memcpy(databuf, &(robosharkstate.depth), 4);
				break;
			
			default:
				break;
		}
		
		if ( s_datatype > READ_IMU2_GYRO && s_datatype < READ_DEPTH) 
			RFLink_sendStruct(FRIEND_ID1, s_datatype, databuf, 2);
		else
			RFLink_sendStruct(FRIEND_ID1, s_datatype, databuf, 4);

	}
}

void data_show_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&DataShowTCB,
							 "Data Show App",
							 data_show_app_task,
							 0,
							 DATA_SHOW_APP_TASK_PRIO,
							 &DATA_SHOW_APP_TASK_STK[0],
							 DATA_SHOW_APP_STK_SIZE/10,
							 DATA_SHOW_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&DataShowTmr,
							"Data Show Timer",
							0,
							DATA_SHOW_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)data_show_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&DataShowTmr,
						&err);
	
	data_show_app_stop();
	
	OS_CRITICAL_EXIT();
}

// 暂停运行
void data_show_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&DataShowTmr,
						OS_OPT_TMR_NONE,
						0,
						&err);

	OSTaskSuspend(&DataShowTCB,
								&err);

	OS_CRITICAL_EXIT();
}


// 恢复运行
void data_show_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&DataShowTCB,
						 &err);

	OSTmrStart(&DataShowTmr,
					 &err);

	OS_CRITICAL_EXIT();
}
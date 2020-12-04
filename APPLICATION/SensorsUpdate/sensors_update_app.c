/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "sensors_update_app.h"
extern BOXFISH boxfishstate;

static OS_TMR SensorsUpdateTmr;
static OS_TCB SensorsUpdateTCB;
__align(8) CPU_STK SENSORS_UPDATE_APP_TASK_STK[SENSORS_UPDATE_APP_STK_SIZE];


static void sensors_update_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&SensorsUpdateTCB,
								OS_OPT_POST_NONE,
								&err);
}

static void sensors_update_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK now_os_tick;
	
	//short gyro[3];  // 2字节
	float pitch,roll,yaw; 		//欧拉角
	uint8_t mpu_cnt = 0;
	uint8_t var_cnt = 0;
	uint8_t rwdata_cnt = 0;
	uint8_t depth_cnt = 0;

	unsigned long sensor_timestamp;
	
	// IMU数据
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	
	// 
	float t_pre, deltaT;
	float ang1_pre , ang2_pre, ang3_pre;
	float angw1, angw2, angw3;
	
	// 深度计数据（刚启动时的）
	float depth_ori;
	depth_ori = get_ms5837_data();
	
	//
	uint8_t databuf[16];
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		// 时间戳
		now_os_tick = OSTimeGet(&err);
		boxfishstate.timestamp = (float)now_os_tick/(float)(OS_CFG_TICK_RATE_HZ);
		
		// IMU(1)数据，外接IMU
		MPU_SetAddr(0x68);
		mpu_set_address(0x68);
		dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);
		//mpu_get_gyronobias_reg(gyro,NULL); 
//		boxfishstate.gimbal_imu_data.gyrox = (float)gyro[0]*0.001064; // 单位是：弧度每秒
//		boxfishstate.gimbal_imu_data.gyroy = (float)gyro[1]*0.001064;
//		boxfishstate.gimbal_imu_data.gyroz = (float)gyro[2]*0.001064;
//		boxfishstate.gimbal_imu_data.accelx = (float)accel[0]*0.000061037; // 单位是：g
//		boxfishstate.gimbal_imu_data.accely = (float)accel[1]*0.000061037;
//		boxfishstate.gimbal_imu_data.accelz = (float)accel[2]*0.000061037;
		while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
		{
			mpu_cnt++;
			if(mpu_cnt>10) 
			{
				mpu_cnt = 0;
				break;
			}
		}
		boxfishstate.gimbal_imu_data.pitch = pitch;
		boxfishstate.gimbal_imu_data.roll = roll;
		boxfishstate.gimbal_imu_data.yaw = yaw;
		
		
//		// IMU(2)数据，板载IMU
//		MPU_SetAddr(0x69);
//		mpu_set_address(0x69);
//		dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);
//		//mpu_get_gyronobias_reg(gyro,NULL); 
//		boxfishstate.onboard_imu_data.gyrox = (float)gyro[0]*0.001064; // 单位是：弧度每秒
//		boxfishstate.onboard_imu_data.gyroy = (float)gyro[1]*0.001064;
//		boxfishstate.onboard_imu_data.gyroz = (float)gyro[2]*0.001064;
//		boxfishstate.onboard_imu_data.accelx = (float)accel[0]*0.000061037; // 单位是：g
//		boxfishstate.onboard_imu_data.accely = (float)accel[1]*0.000061037;
//		boxfishstate.onboard_imu_data.accelz = (float)accel[2]*0.000061037;
//		
//		// 姿态角
//		while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
//		{
//			mpu_cnt++;
//			if(mpu_cnt>10) 
//			{
//				mpu_cnt = 0;
//				break;
//			}
//		}
//		boxfishstate.onboard_imu_data.pitch = pitch;
//		boxfishstate.onboard_imu_data.roll = roll;
//		boxfishstate.onboard_imu_data.yaw = yaw;
		
		// 深度数据
		if(depth_cnt < 10)
			depth_cnt++;
		else
		{
			boxfishstate.depth = get_ms5837_data()-depth_ori;
			depth_cnt = 0;
		}

		
		//BuffPrintf("Sensors Update\n");
		
	}
}

void sensors_update_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&SensorsUpdateTCB,
							 "Sensors Update App",
							 sensors_update_app_task,
							 0,
							 SENSORS_UPDATE_APP_TASK_PRIO,
							 &SENSORS_UPDATE_APP_TASK_STK[0],
							 SENSORS_UPDATE_APP_STK_SIZE/10,
							 SENSORS_UPDATE_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&SensorsUpdateTmr,
							"Sensors Update Timer",
							0,
							SENSORS_UPDATE_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)sensors_update_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&SensorsUpdateTmr,
						&err);
							 
	OS_CRITICAL_EXIT();
}

// 暂停运行
void sensors_update_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&SensorsUpdateTmr,
						OS_OPT_TMR_NONE,
						0,
						&err);

	OSTaskSuspend(&SensorsUpdateTCB,
								&err);

	OS_CRITICAL_EXIT();
}

// 恢复运行
void sensors_update_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&SensorsUpdateTCB,
						 &err);

	OSTmrStart(&SensorsUpdateTmr,
					 &err);

	OS_CRITICAL_EXIT();
}
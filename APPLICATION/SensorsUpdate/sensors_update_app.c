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

short char_to_short(unsigned char cData[])
{
	return ((short)cData[1]<<8)|cData[0];
}

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
	float gyro[3], accel[3];
//	unsigned char more;
//	long quat[4]; 	
//	float t_pre, deltaT;
//	float ang1_pre , ang2_pre, ang3_pre;
//	float angw1, angw2, angw3;
	unsigned char chrTemp[30];
		
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
		JY901_read_bytes(0x50, JY901_AX, 24, &chrTemp[0]);
		accel[0] = (float)char_to_short(&chrTemp[0])/32768*16;
		accel[1] = (float)char_to_short(&chrTemp[2])/32768*16;
		accel[2] = (float)char_to_short(&chrTemp[4])/32768*16;
		gyro[0] = (float)char_to_short(&chrTemp[6])/32768*2000;
		gyro[1] = (float)char_to_short(&chrTemp[8])/32768*2000;
		gyro[2] = (float)char_to_short(&chrTemp[10])/32768*2000;
//		h[0] = CharToShort(&chrTemp[12]);
//		h[1] = CharToShort(&chrTemp[14]);
//		h[2] = CharToShort(&chrTemp[16]);
		roll = (float)char_to_short(&chrTemp[18])/32768*180;
		pitch = (float)char_to_short(&chrTemp[20])/32768*180;
		yaw = (float)char_to_short(&chrTemp[22])/32768*180;
		
		boxfishstate.imu_data.gyrox  = gyro[0]; // 单位是：
		boxfishstate.imu_data.gyroy  = gyro[1];
		boxfishstate.imu_data.gyroz  = gyro[2];
		boxfishstate.imu_data.accelx = accel[0]; // 单位是：g
		boxfishstate.imu_data.accely = accel[1];
		boxfishstate.imu_data.accelz = accel[2];
		boxfishstate.imu_data.pitch  = pitch;
		boxfishstate.imu_data.roll   = roll;
		boxfishstate.imu_data.yaw    = yaw;
		
				
		// 深度数据
		if(depth_cnt < 10)
		{
			depth_cnt++;
		}
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
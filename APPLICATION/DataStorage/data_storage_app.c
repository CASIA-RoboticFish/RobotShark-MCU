/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "data_storage_app.h"
extern BOXFISH boxfishstate;

static OS_TMR DataStorageTmr;
static OS_TCB DataStorageTCB;
__align(8) CPU_STK DATA_STORAGE_APP_TASK_STK[DATA_STORAGE_APP_STK_SIZE];


// ����ָ��Ҫ��¼�������ļ�����
// ����¼���ݣ���Ϊ0
// �������ݣ�����Ϊ1
// �������͵����ݿ�����������ȥ����
static uint8_t datafile_type = 0; 
static unsigned char datafile[32] = "data.bin";

void set_savefile_name(uint8_t ftype, unsigned char* fname)
{
	uint8_t res;
	if(ftype == 1)
	{
		// ע����������ǲ����Զ��ַ�����ַ��ֵ�ģ�һ��Ҫ���ַ�������
		// ��Ҫ�����·������ַ�����ֵ����������ܴ�
		// static unsigned char *datafile;
		// datafile = fname;
		// ���Ϸ��������datafile��ֵ�������ı�
		strcpy(datafile, fname); 
		res = mf_open(datafile, FA_CREATE_ALWAYS);	
		mf_close();
		datafile_type = 1;
	}
}

static void data_storage_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&DataStorageTCB,
								OS_OPT_POST_NONE,
								&err);
}


static void data_storage_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK tick;
	uint8_t res;
	

	uint8_t databuf[32];
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		switch(datafile_type)
		{
			case 1:
				// ���Ǽ�¼�������ݵ��ļ�
				// ϵͳʱ��
				memcpy(databuf, &(boxfishstate.timestamp), 4);
				// pitch
				memcpy(databuf+4, &(boxfishstate.gimbal_imu_data.roll), 4);
				// roll
				memcpy(databuf+8, &(boxfishstate.gimbal_imu_data.pitch), 4);
				// yaw
				memcpy(databuf+12, &(boxfishstate.gimbal_imu_data.yaw), 4);
				// ����
				//*(databuf+20) = 0xff;
				
				res = mf_open(datafile, FA_OPEN_ALWAYS | FA_WRITE);
				
				if(res)
					BuffPrintf("File open failed, error: %d\n", res);
				
				mf_lseek(mf_size());
				mf_write(databuf, DATA_SAVE_LENGTH); // һ֡���ݵĳ�����20�ֽ�
				
				mf_close();
				break;
			default:
				break;
		}

//		BuffPrintf("%f, %f��%f��%f\n", tick,
//																	 boxfishstate.onboard_imu_data.pitch,
//																	 boxfishstate.onboard_imu_data.roll,
//																	 boxfishstate.onboard_imu_data.yaw);

	}
	
}

void data_storage_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&DataStorageTCB,
							 "Data Storage App",
							 data_storage_app_task,
							 0,
							 DATA_STORAGE_APP_TASK_PRIO,
							 &DATA_STORAGE_APP_TASK_STK[0],
							 DATA_STORAGE_APP_STK_SIZE/10,
							 DATA_STORAGE_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&DataStorageTmr,
							"Data Storage Timer",
							0,
							DATA_STORAGE_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)data_storage_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&DataStorageTmr,
						&err);
	
	// ��ͣ�߳�
	data_storage_app_stop();
	
	OS_CRITICAL_EXIT();
}

// ��ͣ����
void data_storage_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&DataStorageTmr,
						OS_OPT_TMR_NONE,
						0,
						&err);

	OSTaskSuspend(&DataStorageTCB,
								&err);

	OS_CRITICAL_EXIT();
	
	// ��ͣ��¼����ʱ��flag��Ϊ0
	datafile_type = 0;
	
}

// �ָ�����
void data_storage_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&DataStorageTCB,
						 &err);

	OSTmrStart(&DataStorageTmr,
					 &err);

	OS_CRITICAL_EXIT();
}
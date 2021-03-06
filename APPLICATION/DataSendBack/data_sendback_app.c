/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "data_sendback_app.h"
extern ROBOSHARK robosharkstate;

static OS_TCB DataSendbackTCB;
__align(8) CPU_STK DATA_SENDBACK_APP_TASK_STK[DATA_SENDBACK_APP_STK_SIZE];


static unsigned char sendfile[32] = "data.bin";

void set_sendfile_name(unsigned char* fname)
{
	// ע����������ǲ����Զ��ַ�����ַ��ֵ�ģ�һ��Ҫ���ַ�������
	// ��Ҫ�����·������ַ�����ֵ����������ܴ�
	// static unsigned char *datafile;
	// datafile = fname;
	// ���Ϸ��������datafile��ֵ�������ı�
	strcpy(sendfile, fname);
}

void go_to_sendback(uint8_t ftype)
{
	uint8_t send_flag = ftype;
	OS_ERR err;
	
	OSTaskQPost(&DataSendbackTCB,
							(void *)send_flag,
							sizeof(send_flag),
							OS_OPT_POST_FIFO,
							&err);
							
	sensors_update_app_stop();
	data_storage_app_stop();
	swim_control_forcestop();
	pantilt_control_stop();
	BuffPrintf_Stop();	
}

static void data_sendback_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE size;
	uint8_t res;
	uint32_t file_size;
	uint8_t send_flag;
	uint32_t check_sum = 0;
	uint8_t datalen = 0;
	
	while(1)
	{
		send_flag = (uint8_t)OSTaskQPend(0,
																		 OS_OPT_PEND_BLOCKING,
																		 &size,
																		 &ts,
																		 &err);
		
		if(err != OS_ERR_NONE)
			continue;
		
		if(!send_flag)
			continue;
			
		// �򿪶�Ӧ�ļ�
		// �򿪻��������ļ�
		res = mf_open(sendfile, FA_READ);
		if(res)
			MyPrintf("File open failed, error: %d\n", res);
		datalen = DATA_SAVE_LENGTH; // ��ʾһ֡���ݵĳ��ȣ�������20�ֽ�
		// �����ļ�����
		file_size = mf_size();
		for(int i = 0; i < (file_size/datalen); i++)
		{
			// ����RFLinkЭ�鷢��
			check_sum = 0;
			MyPrintf_PutChar(0xff);
			check_sum += 0xff;
			MyPrintf_PutChar(MY_ID);
			check_sum += MY_ID;
			MyPrintf_PutChar(FRIEND_ID1);
			check_sum += FRIEND_ID1;
			MyPrintf_PutChar(datalen+1);
			check_sum += (datalen+1);
			MyPrintf_PutChar((uint8_t)GOTO_SEND_DATA);
			check_sum += (uint8_t)GOTO_SEND_DATA;
			MyPrintf_PutChar(send_flag);
			check_sum += send_flag;
			// ��ȡ����
			mf_read(datalen);
			
			for(int j = 0; j < datalen; j++)
			{
				MyPrintf_PutChar(*(fatbuf+j));
				check_sum += *(fatbuf+j);
			}
			check_sum = check_sum % 255;
			MyPrintf_PutChar((uint8_t)check_sum);
			delay_ms(50);
		}
		// �ر��ļ�
		mf_close();
		
		// ���ͽ������
		check_sum = 0;
		MyPrintf_PutChar(0xff);
		check_sum += 0xff;
		MyPrintf_PutChar(MY_ID);
		check_sum += MY_ID;
		MyPrintf_PutChar(FRIEND_ID1);
		check_sum += FRIEND_ID1;
		MyPrintf_PutChar(0x01);
		check_sum += 0x01;
		MyPrintf_PutChar((uint8_t)GOTO_SEND_DATA);
		check_sum += (uint8_t)GOTO_SEND_DATA;
		MyPrintf_PutChar(0xEF);
		check_sum += 0xEF;
		check_sum = check_sum % 255;
		MyPrintf_PutChar((uint8_t)check_sum);
		delay_ms(1000);
		
		// �ָ�����
		sensors_update_app_resume();
		BuffPrintf_Resume();	
		
	}
	
	
}

void data_sendback_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&DataSendbackTCB,
							 "Data Sendback App",
							 data_sendback_app_task,
							 0,
							 DATA_SENDBACK_APP_TASK_PRIO,
							 &DATA_SENDBACK_APP_TASK_STK[0],
							 DATA_SENDBACK_APP_STK_SIZE/10,
							 DATA_SENDBACK_APP_STK_SIZE,
							 DATA_SENDBACK_QUEUE_SIZE,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

							 
	OS_CRITICAL_EXIT();
}

/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
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
	// 注意这个函数是不可以对字符串地址赋值的，一定要是字符串数组
	// 不要用如下方法给字符串赋值，这样问题很大：
	// static unsigned char *datafile;
	// datafile = fname;
	// 以上方法会造成datafile的值经常被改变
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
			
		// 打开对应文件
		// 打开基础数据文件
		res = mf_open(sendfile, FA_READ);
		if(res)
			MyPrintf("File open failed, error: %d\n", res);
		datalen = DATA_SAVE_LENGTH; // 表示一帧数据的长度，这里是20字节
		// 发送文件内容
		file_size = mf_size();
		for(int i = 0; i < (file_size/datalen); i++)
		{
			// 按照RFLink协议发送
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
			// 读取数据
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
		// 关闭文件
		mf_close();
		
		// 发送结束标记
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
		
		// 恢复运行
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

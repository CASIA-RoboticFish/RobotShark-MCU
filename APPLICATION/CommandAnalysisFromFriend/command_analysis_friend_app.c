/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.10.09
History: 
*****************************************************************************/
#include "command_analysis_friend_app.h"

OS_TCB CommandAnalysisFriendTCB;
__align(8) CPU_STK COMMAND_ANALYSIS_FRIEND_TASK_STK[COMMAND_ANALYSIS_FRIEND_STK_SIZE];


void friend_command_execute(Command command, unsigned char* message)
{
	float data1 = 0.0f;
	float data2 = 0.0f;
	float data3 = 0.0f;
	float data4 = 0.0f;
	uint8_t databuf[16];
	
	switch(command)
	{
		case SET_SWIM_RUN:
			swim_control_start();
			break;
			
		case SET_SWIM_STOP:
			swim_control_stop();
			break;

		case LAST_COMMAND_FLAG:
			break;
		
		default:
			break;
	}
}


static void command_analysis_friend_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE size;
	uint8_t rx_data;
	uint8_t ret;
	AnalysisResult result;
	while(1)
	{

		rx_data = (uint8_t)OSTaskQPend(0,
					  OS_OPT_PEND_BLOCKING,
						&size,
						&ts,
						&err);
		
		if(err != OS_ERR_NONE)
		{
			continue;
		}
		
		if(RFLink_byteAnalysisCall(USART1_ID, rx_data))
		{
			//BuffPrintf("Package analysised\n");
			result =  RFLink_commandAnalysis(USART1_ID);
 			friend_command_execute(result.command, result.message);
		}
		
	}
}


void command_analysis_friend_app_init( void )
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&CommandAnalysisFriendTCB,
		 "Command Analysis friend App",
		 command_analysis_friend_app_task,
		 0,
		 COMMAND_ANALYSIS_FRIEND_TASK_PRIO,
		 &COMMAND_ANALYSIS_FRIEND_TASK_STK[0],
		 COMMAND_ANALYSIS_FRIEND_STK_SIZE/10,
		 COMMAND_ANALYSIS_FRIEND_STK_SIZE,
		 COMMAND_ANALYSIS_FRIEND_QUEUE_SIZE,
		 0,
		 0,
		 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		 &err);


	OS_CRITICAL_EXIT();
} 




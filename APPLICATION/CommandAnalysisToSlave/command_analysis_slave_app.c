/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "command_analysis_slave_app.h"

OS_TCB CommandAnalysisSlaveTCB;
__align(8) CPU_STK COMMAND_ANALYSIS_SLAVE_TASK_STK[COMMAND_ANALYSIS_SLAVE_STK_SIZE];

extern ROBOTARGET robotarget;

void slave_command_execute(Command command, unsigned char* message)
{
	float data1 = 0.0f;
	float data2 = 0.0f;
	float data3 = 0.0f;
	float data4 = 0.0f;
	uint8_t databuf[16];
	
	switch(command)
	{
		case SET_FLYWHEEL_RUN:
			RFLink_message("flyon");
			break;
		
		case SET_FLYWHEEL_STOP:
			RFLink_message("flyoff");
			break;
		
		case SET_FLYWHEEL_DATA:
			memcpy(&data1, message, 4);
			RFLink_message("%f", data1);
			break;
		
		case SET_FLYWHEEL_CMD:
			memcpy(&data1, message, 4);
			memcpy(&data2, message+4, 4);
			memcpy(&data3, message+8, 4);
			memcpy(&data4, message+12, 4);
			RFLink_message("%f,%f,%f,%f", data1, data2, data3, data4);
			break;
		
		case SET_TARGET_POS:
			memcpy(&robotarget.image_xpos, message, 4);
			memcpy(&robotarget.image_ypos, message+4, 4);
			break;
			
		case LAST_COMMAND_FLAG:
			break;
		
		default:
			break;
	}
}


static void command_analysis_slave_app_task(void * p_arg)
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
		
		if(RFLink_byteAnalysisCall(USART6_ID, rx_data))
		{
			//BuffPrintf("Package analysised\n");
			result =  RFLink_commandAnalysis(USART6_ID);
 			slave_command_execute(result.command, result.message);
		}
		
	}
}


void command_analysis_slave_app_init( void )
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&CommandAnalysisSlaveTCB,
							 "Command Analysis Slave App",
							 command_analysis_slave_app_task,
							 0,
							 COMMAND_ANALYSIS_SLAVE_TASK_PRIO,
							 &COMMAND_ANALYSIS_SLAVE_TASK_STK[0],
							 COMMAND_ANALYSIS_SLAVE_STK_SIZE/10,
							 COMMAND_ANALYSIS_SLAVE_STK_SIZE,
							 COMMAND_ANALYSIS_SLAVE_QUEUE_SIZE,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);


	OS_CRITICAL_EXIT();
} 




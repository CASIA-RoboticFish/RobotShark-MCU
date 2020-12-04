/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "disk_attach_app.h"

extern BOXFISH boxfishstate;


OS_TCB DiskAttachTCB;
__align(8) CPU_STK DISK_ATTACH_TASK_STK[DISK_ATTACH_STK_SIZE];


void go_to_attach(void)
{
	uint8_t attach_flag;
	attach_flag = ATTACH;
	OS_ERR err;
	OSTaskQPost(&DiskAttachTCB,
							(void *)attach_flag,
							sizeof(attach_flag),
							OS_OPT_POST_FIFO,
							&err);
}


void go_to_detach(void)
{
	uint8_t attach_flag;
	attach_flag = DETACH;
	OS_ERR err;
	OSTaskQPost(&DiskAttachTCB,
							(void *)attach_flag,
							sizeof(attach_flag),
							OS_OPT_POST_FIFO,
							&err);
}


static void disk_attach_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE size;
	uint8_t attach_flag;
	uint8_t tim_cnt = 0;
	while(1)
	{

		attach_flag = (uint8_t)OSTaskQPend(0,
																  OS_OPT_PEND_BLOCKING,
																	&size,
																	&ts,
																	&err);
		
		if(err != OS_ERR_NONE)
    {
			continue;
    }
		
		switch(attach_flag)
		{
			case ATTACH:
				valve_on(VALVE1);
				boxfishstate.valve1_state = VALVE_ON;
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
				valve_off(VALVE2);
				boxfishstate.valve2_state = VALVE_OFF;
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
				pump_on(0);
				boxfishstate.pump_state = PUMP_ON;
				while(1)
				{
					tim_cnt++;
					if(tim_cnt > 100)
					{
						pump_off(0);
						boxfishstate.pump_state = PUMP_OFF;
						valve_off(VALVE1);
						boxfishstate.valve1_state = VALVE_OFF;
						tim_cnt = 0;
						break;
					}
					if(boxfishstate.varistor2_data < 3000)
					{
						pump_off(0);
						boxfishstate.pump_state = PUMP_OFF;
						valve_off(VALVE1);
						boxfishstate.valve1_state = VALVE_OFF;
						tim_cnt = 0;
						break;
					}
					OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
				}
				//BuffPrintf("attach\n");
				break;
				
			case DETACH:
				valve_on(VALVE2);
				boxfishstate.valve2_state = VALVE_ON;
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
				valve_off(VALVE1);
				boxfishstate.valve1_state = VALVE_OFF;
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
				pump_on(0);
			  boxfishstate.pump_state = PUMP_ON;
				while(1)
				{
					tim_cnt++;
					if(tim_cnt > 50)
					{
						pump_off(0);
						boxfishstate.pump_state = PUMP_OFF;
						valve_off(VALVE2);
						boxfishstate.valve2_state = VALVE_OFF;
						tim_cnt = 0;
						break;
					}
					OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
				}
				//  BuffPrintf("detach\n");
				break;
			default:
				break;
		}
		
	}
}

void disk_attach_app_init ( void )
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&DiskAttachTCB,
							 "Disk Attach App",
							 disk_attach_app_task,
							 0,
							 DISK_ATTACH_TASK_PRIO,
							 &DISK_ATTACH_TASK_STK[0],
							 DISK_ATTACH_STK_SIZE/10,
							 DISK_ATTACH_STK_SIZE,
							 DISK_ATTACH_QUEUE_SIZE,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);


	OS_CRITICAL_EXIT();
} 
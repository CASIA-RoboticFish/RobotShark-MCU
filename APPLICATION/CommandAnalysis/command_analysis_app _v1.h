/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _COMMAND_ANALYSIS_APP_H_
#define _COMMAND_ANALYSIS_APP_H_
#include "stm32f4xx.h"
#include "includes.h"
#include "delay.h"

#include "valve.h"
#include "pump.h"

#include "rflink.h"
#include "BuffPrintf.h"
#include "disk_attach_app.h"
#include "data_storage_app.h"
#include "data_sendback_app.h"
#include "data_show_app.h"
#include "pantilt_control.h"

#include "roboremora.h"



// 任务定义
#define COMMAND_ANALYSIS_TASK_PRIO 7
#define COMMAND_ANALYSIS_STK_SIZE 256
#define COMMAND_ANALYSIS_QUEUE_SIZE 64

//#define PRINT_COMMAND_EN

void command_analysis_app_init ( void );
void command_execute(Command command, unsigned char* message);


#endif
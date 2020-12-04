/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _COMMAND_ANALYSIS_SLAVE_APP_H_
#define _COMMAND_ANALYSIS_SLAVE_APP_H_
#include "stm32f4xx.h"
#include "includes.h"
#include "delay.h"

#include "rflink.h"
#include "BuffPrintf.h"

#include "robotstate.h"



// 任务定义
#define COMMAND_ANALYSIS_SLAVE_TASK_PRIO 8
#define COMMAND_ANALYSIS_SLAVE_STK_SIZE 256
#define COMMAND_ANALYSIS_SLAVE_QUEUE_SIZE 64


void command_analysis_slave_app_init ( void );
void slave_command_execute(Command command, unsigned char* message);


#endif
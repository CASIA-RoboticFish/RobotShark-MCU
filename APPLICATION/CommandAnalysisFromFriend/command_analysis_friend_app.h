/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.10.09
History: 
*****************************************************************************/
#ifndef _COMMAND_ANALYSIS_APP_H_
#define _COMMAND_ANALYSIS_APP_H_
#include "stm32f4xx.h"
#include "includes.h"
#include "delay.h"

#include "rflink.h"
#include "BuffPrintf.h"

#include "robotstate.h"



// 任务定义
#define COMMAND_ANALYSIS_FRIEND_TASK_PRIO 13
#define COMMAND_ANALYSIS_FRIEND_STK_SIZE 256
#define COMMAND_ANALYSIS_FRIEND_QUEUE_SIZE 64


void command_analysis_friend_app_init ( void );
void friend_command_execute(Command command, unsigned char* message);


#endif
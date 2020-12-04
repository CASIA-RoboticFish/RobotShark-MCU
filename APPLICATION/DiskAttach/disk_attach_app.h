/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _DISK_ATTACH_APP_H_
#define _DISK_ATTACH_APP_H_
#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"

#include "valve.h"
#include "pump.h"

#include "BuffPrintf.h"
#include "robotstate.h"
// 任务定义
#define DISK_ATTACH_TASK_PRIO 5
#define DISK_ATTACH_STK_SIZE 128
#define DISK_ATTACH_QUEUE_SIZE 8

#define ATTACH 0x01
#define DETACH 0x02

void go_to_attach(void);
void go_to_detach(void);
void disk_attach_app_init(void);
#endif

/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef _DATA_SENDBACK_APP_H_
#define _DATA_SENDBACK_APP_H_

#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
#include "string.h"

#include "malloc.h" 
#include "ff.h"
#include "exfuns.h"
#include "fattester.h"

#include "rflink.h"
#include "MyPrintf.h"
#include "BuffPrintf.h"
#include "swim_control_app.h"
#include "data_storage_app.h"
#include "sensors_update_app.h"

#include "robotstate.h"
#include "config.h"

// 任务定义
#define DATA_SENDBACK_APP_TASK_PRIO 12
#define DATA_SENDBACK_APP_STK_SIZE 512
#define DATA_SENDBACK_QUEUE_SIZE 8

void data_sendback_app_init(void);
void go_to_sendback(uint8_t ftype);
void set_sendfile_name(unsigned char* fname);

#endif
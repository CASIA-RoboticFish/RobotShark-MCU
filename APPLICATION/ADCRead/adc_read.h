/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __ADC_READ_NEW_H_
#define __ADC_READ_NEW_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#include "adc.h"
#include "BuffPrintf.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#include "os_cfg_app.h"
#endif

/********************************************************************************
*
* 任务配置区域
*
********************************************************************************/
// 配置云台控制任务
#define ADC_READ_TASK_PRIO 4
#define ADC_READ_STK_SIZE 256
void adc_read_task(void *p_arg);
void adc_read_task_init();
#endif

/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#ifndef __JY901_TEST_H_
#define __JY901_TEST_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#include "pwm.h"
#include "JY901.h"
#include "BuffPrintf.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#include "os_cfg_app.h"
#endif


/********************************************************************************
*
* 外部变量申明区域
*
********************************************************************************/
extern struct SIMUDATA stcIMU[2];

/********************************************************************************
*
* 任务配置区域
*
********************************************************************************/

// 配置JY901测试任务
#define JY901_TEST_TASK_PRIO 4
#define JY901_TEST_STK_SIZE 256
void jy901_test_task(void *p_arg);
void jy901_test_task_init();



#endif
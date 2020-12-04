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
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#include "os_cfg_app.h"
#endif

/********************************************************************************
*
* ������������
*
********************************************************************************/
// ������̨��������
#define ADC_READ_TASK_PRIO 4
#define ADC_READ_STK_SIZE 256
void adc_read_task(void *p_arg);
void adc_read_task_init();
#endif

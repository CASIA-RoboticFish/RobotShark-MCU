/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __E18_H
#define __E18_H

#include "stm32f4xx.h"
#include "includes.h"
#include "os_cfg_app.h"
#include "sys.h"
#include "led.h"


//#define             FIND_OBSTACLE               1
//#define             FIND_NO_OBSTACLE            0
#define             E18_AHEAD                   1
#define             E18_LEFT                    2
#define             E18_RIGHT                   3


void E18_init();
u8 E18_detect(u8 channel);
void detect_filtering();				// ���ϰ�����ֵ������˲� ����δ��ɣ�

#endif
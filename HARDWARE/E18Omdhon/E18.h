/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
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


#define             FIND_OBSTACLE               1
#define             FIND_NO_OBSTACLE            0
#define             E18_forward                 GPIO_Pin_4				//GPIOB4
#define             E18_left                    GPIO_Pin_5				//GPIOB5
#define             E18_right                   GPIO_Pin_0				//GPIOB0
#define             ForwardDetect               GPIO_ReadInputDataBit(GPIOB, E18_forward)		//循环调用定义的变量就可读取当前检测障碍的结果
#define             LeftDetect                  GPIO_ReadInputDataBit(GPIOB, E18_left)
#define             RightDetect                 GPIO_ReadInputDataBit(GPIOB, ForwardDetect)



void E18_init();
void detect_filtering()；				// 对障碍检测二值化结果滤波 （还未完成）

#endif
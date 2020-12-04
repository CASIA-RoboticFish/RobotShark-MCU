/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: Yang Xiang (Shawn)
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
/*
 * =====================================================================================
 *
 *       Filename:  MyMath.h
 *
 *    Description:  The header of MyMath.c
 *
 *        Version:  1.0
 *        Created:  2015/9/16 17:31:22
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/9/16 17:31:22
 *
 * =====================================================================================
 */
#ifndef _MY_MATH_H_
#define _MY_MATH_H_

#include "arm_math.h"
float MyMath_Cal3DModulus ( float x, float y, float z );
float MyMath_ApplyDeadBandF32 ( float data, float dead_band );
float MyMath_ConstrainF32 ( float data, float low_limit, float high_limit );
float MyMath_SignF32(float f);
int MyMath_FloatToInt(float target);

#endif

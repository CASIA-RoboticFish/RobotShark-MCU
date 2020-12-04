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
 *       Filename:  cpg.h
 *
 *    Description:  The header of cpg.c
 *
 *        Version:  1.0
 *        Created:  2016/7/7 16:03:43
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK V4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2016/7/7 16:03:43
 *
 * =====================================================================================
 */
#ifndef _CPG_H_
#define _CPG_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "arm_math.h"

#define CPG_MAX_NODE_NUM         4
//#define CPG_FREQ_SMOOTH

struct CPGNode{
    // Inputs
    float target_amp;
    float offset;

    // Outputs
    float angle;

    // Private params
    float intri_amp[2];
    float amp[2];
    float amp_first_order[2];
    float phase[2];
    float phase_first_order[2];
};

struct CPGModel{
    int node_num;

    float target_freq;
    float phase_diff;

    float smooth_freq;
    struct CPGNode nodes[CPG_MAX_NODE_NUM];
};

extern const float kDefaultFreq;
extern const float kDefaultAmp;
extern const float kDefaultOffset;
extern const float kDefaultPhaseDiffRatio;

void CPG_UpdateModel(struct CPGModel * model, float time_interval);
void CPG_SetPhaseDiff(struct CPGModel * model, float phase_diff_ratio);
void CPG_SetFrequency(struct CPGModel * model, float freq);
void CPG_SetAmplitude(struct CPGModel * model, int joint, float amp);
void CPG_SetOffset(struct CPGModel * model, int joint, float offset);
void CPG_Reset(struct CPGModel * model);
void CPG_InitModelDefault(struct CPGModel * model, \
                          int node_num);
void CPG_SetOffsetRate(struct CPGModel * model, float rate);
void CPG_SetAmplitudeRate(struct CPGModel * model, float rate);

#endif /* ifndef _CPG_H_ */

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
 *       Filename:  cpg.c
 *
 *    Description:  The module carrying out the CPG model caculation.
 *
 *        Version:  1.0
 *        Created:  2016/7/7 16:04:14
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK V4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2016/7/7 16:04:14
 *
 * =====================================================================================
 */
#include "cpg.h"
#include "MyMath.h"

const float kCPGCalcStepSize = 0.001f;
const float kMaxDeltaFreq = 0.05f;
const float kCPGAmpCoef = 10.0f;
const float kCPGCoup = 4.0f;

const float kDefaultFreq = 1.0f;
const float kDefaultAmp = 0.0f;
const float kDefaultOffset = 0.0f;
const float kDefaultPhaseDiffRatio = 1.0f / 5.0f;
const float kInitInnerPhaseDiff = 2 * PI * 0.01f;

const float kCPGAmp[3] = {20 * PI / 180, 29 * PI / 180, 38 * PI / 180};

/**
 * @brief  Update the CPG model for one sigle calculation step.
 *
 * @param  model
 * @param  calc_step_size
 */
void CPG_UpdateModelOneStep(struct CPGModel * model, float calc_step_size){
    float delta_freq;
    float freq_err;
    float omega;
    int joint, lr;
    float coup_value;
    float amp_second_order;
    struct CPGNode * cur_node = NULL;

#ifdef CPG_FREQ_SMOOTH
    if(model->smooth_freq < model->target_freq){
        delta_freq = calc_step_size * kMaxDeltaFreq;
        freq_err = model->target_freq - model->smooth_freq;
        if(fabs(freq_err) > delta_freq){
            model->smooth_freq += delta_freq * MyMath_SignF32(freq_err);
        }else{
            model->smooth_freq = model->target_freq;
        }
    }else{
        model->smooth_freq = model->target_freq;
    }
#else
    model->smooth_freq = model->target_freq;
#endif

    omega = 2 * PI * model->smooth_freq;

    for(joint = 0; joint < model->node_num; joint++){
        cur_node = &(model->nodes[joint]);

        cur_node->intri_amp[0] = 0.5f * (cur_node->target_amp + cur_node->offset);
        cur_node->intri_amp[1] = 0.5f * (cur_node->target_amp - cur_node->offset);

        coup_value = kCPGCoup * arm_sin_f32(cur_node->phase[1] - cur_node->phase[0] - PI);
        cur_node->phase_first_order[0] = omega + coup_value;
        cur_node->phase_first_order[1] = omega - coup_value;

        for(lr = 0; lr < 2; lr++){
            amp_second_order = kCPGAmpCoef * \
                               (0.25f * \
                                kCPGAmpCoef * \
                                (cur_node->intri_amp[lr] - \
                                 cur_node->amp[lr]) - \
                                cur_node->amp_first_order[lr]);
            cur_node->amp_first_order[lr] += calc_step_size * amp_second_order;
            cur_node->amp[lr] += calc_step_size * cur_node->amp_first_order[lr];

            if(joint == 0){
                cur_node->phase_first_order[lr] += kCPGCoup * \
                                                   arm_sin_f32(model->nodes[joint+1].phase[lr] - \
                                                               model->nodes[joint].phase[lr] + \
                                                               model->phase_diff);
            }else if(joint == model->node_num - 1){
                cur_node->phase_first_order[lr] += kCPGCoup * \
                                                   arm_sin_f32(model->nodes[joint-1].phase[lr] - \
                                                               model->nodes[joint].phase[lr] - \
                                                               model->phase_diff);
            }else{
                cur_node->phase_first_order[lr] += kCPGCoup * \
                                                   (arm_sin_f32(model->nodes[joint+1].phase[lr] - \
                                                                model->nodes[joint].phase[lr] + \
                                                                model->phase_diff) +\
                                                    arm_sin_f32(model->nodes[joint-1].phase[lr] - \
                                                                model->nodes[joint].phase[lr] - \
                                                                model->phase_diff));
            }
            cur_node->phase[lr] += calc_step_size * cur_node->phase_first_order[lr];
        }
        cur_node->angle = cur_node->amp[0] * (1.0f + arm_cos_f32(cur_node->phase[0])) - \
                          cur_node->amp[1] * (1.0f + arm_cos_f32(cur_node->phase[1]));
//        cur_node->angle = cur_node->amp[1] * arm_cos_f32(cur_node->phase[1]);
    }
}

/**
 * @brief  Update the CPG model for multi calculation steps.
 *
 * @param  model
 * @param  calc_step_size
 * @param  steps
 */
void CPG_UpdateModelMultiSteps(struct CPGModel * model, float calc_step_size, int steps){
    int i = 0;
    for(i = 0; i < steps; i++){
        CPG_UpdateModelOneStep(model, calc_step_size);
    }
}

/**
 * @brief  Update the CPG Model for a specific time interval.
 *
 * @param  model
 * @param  time_interval
 */
void CPG_UpdateModel(struct CPGModel * model, float time_interval){
    float steps_f32 = 0.0f;
    int steps = 0;
    steps_f32 = time_interval / kCPGCalcStepSize;
    steps = MyMath_FloatToInt(steps_f32);
    CPG_UpdateModelMultiSteps(model, kCPGCalcStepSize, steps);
}

/**
 * @brief  Set the phase difference of CPG Model.
 *
 * @param  model
 * @param  phase_diff_ratio
 */
void CPG_SetPhaseDiffRate(struct CPGModel * model, float phase_diff_ratio){
    model->phase_diff = 2 * PI * phase_diff_ratio;
}

void CPG_SetPhaseDiff(struct CPGModel * model, float phase_diff){
    model->phase_diff = phase_diff;
}

/**
 * @brief  Set the frequency of CPG Model
 *
 * @param  model
 * @param  freq
 */
void CPG_SetFrequency(struct CPGModel * model, float freq){
    model->target_freq = freq;
}

/**
 * @brief  Set the amplitude (specify by "amp") of node (specify by "joint")
 *         of CPG Model.
 *
 * @param  model
 * @param  joint
 * @param  amp
 */
void CPG_SetAmplitude(struct CPGModel * model, int joint, float amp){
    if(joint >= model->node_num){
        return;
    }

    model->nodes[joint].target_amp = amp;
}

/**
 * @brief  Set the offset (specify by "offset") of node (specify by "joint")
 *         of CPG Model.
 *
 * @param  model
 * @param  joint
 * @param  offset
 */
void CPG_SetOffset(struct CPGModel * model, int joint, float offset){
    if(joint >= model->node_num){
        return;
    }

    model->nodes[joint].offset = offset;
}

/**
 * @brief  Reset all the middle data in CPG Model.
 *
 * @param  model
 */
void CPG_Reset(struct CPGModel * model){
    int joint, lr;

    for(joint = 0; joint < model->node_num; joint++){
        for(lr = 0; lr < 2; lr++){
            model->nodes[joint].phase[lr] = 0.0f;
            model->nodes[joint].phase_first_order[lr] = 0.0f;

            model->nodes[joint].amp[lr] = 0.0f;
            model->nodes[joint].amp_first_order[lr] = 0.0f;
        }
        // ATTENTION:
        // There has to be a phase difference in the reset state,
        // or it can not oscillate correctly.
        model->nodes[joint].phase[1] = model->nodes[joint].phase[0] + kInitInnerPhaseDiff;
    }

    model->smooth_freq = kDefaultFreq;
}

void CPG_SetOffsetRate(struct CPGModel * model, float rate){
    int i = 0;

    for(i = 0; i < 3; i++){
        CPG_SetOffset(model, i, kCPGAmp[i] * rate);
    }
}

void CPG_SetAmplitudeRate(struct CPGModel * model, float rate){
    int i = 0;
    for(i = 0; i < 3; i++){
        CPG_SetAmplitude(model, i, kCPGAmp[i] * rate);
    }
}

/**
 * @brief  Init the CPG Model with the default params.
 *
 * @param  model
 * @param  node_num
 */
void CPG_InitModelDefault(struct CPGModel * model, \
                          int node_num){
    int i = 0;

    memset(model, 0, sizeof(struct CPGModel));

    model->node_num = node_num;
    model->target_freq = kDefaultAmp;
    model->smooth_freq = kDefaultFreq;

    model->phase_diff = 2 * PI * kDefaultPhaseDiffRatio;

    for(i = 0; i < node_num; i++){
        model->nodes[i].target_amp = kDefaultAmp;
        model->nodes[i].offset = kDefaultOffset;
    }

    CPG_SetAmplitudeRate(model, 1);
    CPG_Reset(model);
}

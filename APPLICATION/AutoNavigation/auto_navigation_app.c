/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "auto_navigation_app.h"

extern ROBOSHARK robosharkstate;
StateTransform stateTrans[] = {
    //自由漫步情况
    {stop, NORMAL, 0., .2, forward_f, ff},
    {stop, NORMAL, .2, .4, forward_s, fs},
    {stop, NORMAL, .4, .5, yaw_lr, lr},
    {stop, NORMAL, .5, .6, yaw_rr, rr},
    {stop, NORMAL, .6, .75, yaw_ls, ls},
    {stop, NORMAL, .75, .9, yaw_rs, rs},
    {stop, NORMAL, .9, .95, pitch_u, up},
    {stop, NORMAL, .95, 1., pitch_d, down},
    {forward_f, NORMAL, 0., .1, stop, sp},
    {forward_f, NORMAL, .1, .4, forward_s, fs},
    {forward_f, NORMAL, .4, .5, yaw_lr, lr},
    {forward_f, NORMAL, .5, .6, yaw_rr, rr},
    {forward_f, NORMAL, .6, .75, yaw_ls, ls},
    {forward_f, NORMAL, .75, .9, yaw_rs, rs},
    {forward_f, NORMAL, .9, .95, pitch_u, up},
    {forward_f, NORMAL, .95, 1., pitch_d, down},
    {forward_s, NORMAL, 0., .25, stop, sp},
    {forward_s, NORMAL, .25, .4, forward_f, ff},
    {forward_s, NORMAL, .4, .5, yaw_lr, lr},
    {forward_s, NORMAL, .5, .6, yaw_rr, rr},
    {forward_s, NORMAL, .6, .75, yaw_ls, ls},
    {forward_s, NORMAL, .75, .9, yaw_rs, rs},
    {forward_s, NORMAL, .9, .95, pitch_u, up},
    {forward_s, NORMAL, .95, 1., pitch_d, down},
    {yaw_lr, NORMAL, 0., .5, stop, sp},
    {yaw_lr, NORMAL, .5, 1., forward_f, ff}, 
    {yaw_rr, NORMAL, 0., .5, stop, sp},    
    {yaw_rr, NORMAL, .5, 1., forward_f, ff},
    {yaw_ls, NORMAL, 0., .5, stop, sp},
    {yaw_ls, NORMAL, .5, 1., forward_s, fs}, 
    {yaw_rs, NORMAL, 0., .5, stop, sp},    
    {yaw_rs, NORMAL, .5, 1., forward_s, fs},
    {pitch_d, NORMAL, 0., 1., stop, sp},
    {pitch_u, NORMAL, 0., 1., stop, sp},
    
    // 非自由漫步情况     
    {NULL, NNNR, 0., .15, stop, sp},
    //{NULL, NNNR, , , forward_f, ff},
    {NULL, NNNR, .15, .3, forward_s, fs},
    {NULL, NNNR, .3, .7, yaw_lr, lr},
    {NULL, NNNR, .7, 1., yaw_ls, ls},
    
    {NULL, NNLN, 0., .15, stop, sp},
    //{NULL, NNLN, , , forward_f, ff},
    {NULL, NNLN, .15, .3, forward_s, fs},
    {NULL, NNLN, .3, .7, yaw_rr, rr},
    {NULL, NNLN, .7, 1., yaw_rs, rs},
    
    {NULL, NNLR, 0., .5, stop, sp},
    //{NULL, NNLR, , , forward_f, ff},
    {NULL, NNLR, .5, 1., forward_s, fs},
    
    {NULL, NDNN, 0., .1, stop, sp},
    {NULL, NDNN, .1, .15, forward_f, ff},
    {NULL, NDNN, .15, .35, forward_s, fs},
    {NULL, NDNN, .35, .45, yaw_lr, lr},
    {NULL, NDNN, .45, .55, yaw_rr, rr},
    {NULL, NDNN, .55, .65, yaw_ls, ls},
    {NULL, NDNN, .65, .75, yaw_rs, rs},
    {NULL, NDNN, .75, 1., pitch_u, up},
    
    {NULL, NDNR, 0., .15, stop, sp},
    //{NULL, NDNR, , , forward_f, ff},
    {NULL, NDNR, .15, .3, forward_s, fs},
    {NULL, NDNR, .3, .7, yaw_lr, lr},
    {NULL, NDNR, .7, 1., yaw_ls, ls},
    
    {NULL, NDLN, 0., .15, stop, sp},
    //{NULL, NDLN, , , forward_f, ff},
    {NULL, NDLN, .15, .3, forward_s, fs},
    {NULL, NDLN, .3, .7, yaw_rr, rr},
    {NULL, NDLN, .7, 1., yaw_rs, rs},
    
    {NULL, NDLR, 0., .3, stop, sp},
    {NULL, NDLR, .3, .7, forward_s, fs},
    {NULL, NDLR, .7, 1., pitch_u, up},
    
    {NULL, FNNN, 0., .5, yaw_lr, lr},
    {NULL, FNNN, .5, 1., yaw_rr, rr},
    
    {NULL, FNNR, 0., 1., yaw_lr, lr},
    
    {NULL, FNLN, 0., 1., yaw_rr, rr},
    
    {NULL, FNLR, 0., 1., stop, sp},
    
    {NULL, FDNN, 0., .5, yaw_lr, lr},
    {NULL, FDNN, .5, 1., yaw_rr, rr},
    
    {NULL, FDNR, 0., 1., yaw_lr, lr}, 
    
    {NULL, FDLN, 0., 1., yaw_rr, rr},
    
    {NULL, FDLR, 0., 1., stop, sp}
};




void turn_lr(float freq, float offset)
{
    set_tail_freq(freq);
    set_tail_offset(offset);
}

/************************************************************************************************
函数功能：推算当前的状态转换条目
Note: StateTransform形式  {当前状态， 转移概率下限，转移概率上限，事件id， 下一状态， 下一状态下的动作}
输入： 给定的状态转换总表 pSM， 当前事件 evt
输出： 查询到的状态转移条目  或者  NULL
注意： 针对是当前事件是否在自由漫步环境下，进行划分。不是自由漫步的情况下，机器鱼只需要保证左转、右转动作间加入一个缓慢直游地操作过渡。
剩余情况只需要按照转移概率进行查询。自由漫步根据实验效果目前先按照非自由漫步设置。
************************************************************************************************/
StateTransform* findTrans(StateMachine* pSM, const EventID evt)
{
    int i;
    float rand_prob = (rand() % 100) / 100.;
//    float rand_period = rand() % 5 + 1.;
    for (i=0; i < pSM->transNum; i++)
    {
        if((pSM->transform[i].eventId == evt) && evt != NORMAL)
        {
            if((((pSM->transform[i].nextMotionState == yaw_lr) || (pSM->transform[i].nextMotionState == yaw_ls)) && robosharkstate.swim_param.motion_offset > 0.) &&
                (((pSM->transform[i].nextMotionState == yaw_rr) || (pSM->transform[i].nextMotionState == yaw_rs)) && robosharkstate.swim_param.motion_offset < 0.))
            {
                StateTransform tempTrans[] = {{pSM->state, 0., 1., evt, forward_s, fs}};     //当前状态不是NULL 表示是过渡情况
                return &tempTrans[0];
            }
            else if((rand_prob >= pSM->transform[i].transProb_d) && (rand_prob < pSM->transform[i].transProb_u))
            {
                return &pSM->transform[i];
            }
        }
        else if ((pSM->transform[i].eventId == evt) && evt == NORMAL)
        {
            if((((pSM->transform[i].nextMotionState == yaw_lr) || (pSM->transform[i].nextMotionState == yaw_ls)) && robosharkstate.swim_param.motion_offset > 0.) &&
                (((pSM->transform[i].nextMotionState == yaw_rr) || (pSM->transform[i].nextMotionState == yaw_rs)) && robosharkstate.swim_param.motion_offset < 0.))
            {
                StateTransform tempTrans[] = {{pSM->state, 0., 1., evt, forward_s, fs}};   //当前状态不是NULL 表示是过渡情况
                return &tempTrans[0];
            }
            else if((rand_prob >= pSM->transform[i].transProb_d) && (rand_prob < pSM->transform[i].transProb_u))
            {
                return &pSM->transform[i];
            }
        }
    }
    return NULL;
}

void runStateMachine(StateMachine* pSM, const EventID evt)
{
    StateTransform* pTrans;
    pTrans = findTrans(pSM, evt);
    
    if (pTrans == NULL)
    {
        BuffPrintf("Current state %s has a blank schedule, please check the trans table.", pSM->state);
        return;
    }
    
    pSM->state = pTrans->nextMotionState;
    Action act = pTrans->action;
    if (act == NULL) 
    {
        BuffPrintf("Current state %s corresonds to no action, please check the trans table.", pSM->state);
        return;
    }
    BuffPrintf("Current state is %s, the Event is %s, the Action is %s", pSM->state, evt, pTrans->action);
}





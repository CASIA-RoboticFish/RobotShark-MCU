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
static OS_TMR AutoNavigationTmr;  //定义一个定时器
static OS_TCB AutoNavigationTCB;    //定义一个任务块
__align(8) CPU_STK AUTO_NAVIGATION_APP_TASK_STK[AUTO_NAVIGATION_APP_STK_SIZE];  //定义任务堆栈
static int AN_tmrcount = 0;
static int rand_period = 0;
static EventID last_evt = NORMAL;
EventID curEvt = NORMAL;
MotionState *dis_state[] = {"stop", "ff", "fs", "lr", "rr", "ls", "rs", "up", "down"};
Action *dis_action[] = {"sp",  "ff", "fs", "lr", "rr", "ls", "rs", "up", "down"};

unsigned char obs_down = 0x00, obs_ahead = 0x00, obs_left = 0x00, obs_right = 0x00;

StateTransform stateTrans[] = {
    //自由漫步情况
    {stop, NORMAL, 0., .2, forward_f, ff},
    {stop, NORMAL, .2, .4, forward_s, fss},
    {stop, NORMAL, .4, .5, yaw_lr, lr},
    {stop, NORMAL, .5, .6, yaw_rr, rr},
    {stop, NORMAL, .6, .75, yaw_ls, ls},
    {stop, NORMAL, .75, .9, yaw_rs, rs},
    {stop, NORMAL, .9, .95, pitch_u, up},
    {stop, NORMAL, .95, 1., pitch_d, down},
    {forward_f, NORMAL, 0., .1, stop, sp},
    {forward_f, NORMAL, .1, .4, forward_s, fss},
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
    {yaw_ls, NORMAL, .5, 1., forward_s, fss}, 
    {yaw_rs, NORMAL, 0., .5, stop, sp},    
    {yaw_rs, NORMAL, .5, 1., forward_s, fss},
    {pitch_d, NORMAL, 0., 1., stop, sp},
    {pitch_u, NORMAL, 0., 1., stop, sp},
    
    // 非自由漫步情况     
    {NULL, NNNR, 0., .15, stop, sp},
    //{NULL, NNNR, , , forward_f, ff},
    {NULL, NNNR, .15, .3, forward_s, fss},
    {NULL, NNNR, .3, .7, yaw_lr, lr},
    {NULL, NNNR, .7, 1., yaw_ls, ls},
    {NULL, NNLN, 0., .15, stop, sp},
    //{NULL, NNLN, , , forward_f, ff},
    {NULL, NNLN, .15, .3, forward_s, fss},
    {NULL, NNLN, .3, .7, yaw_rr, rr},
    {NULL, NNLN, .7, 1., yaw_rs, rs},
    {NULL, NNLR, 0., .5, stop, sp},
    //{NULL, NNLR, , , forward_f, ff},
    {NULL, NNLR, .5, 1., forward_s, fss},
    {NULL, NDNN, 0., .1, stop, sp},
    {NULL, NDNN, .1, .15, forward_f, ff},
    {NULL, NDNN, .15, .35, forward_s, fss},
    {NULL, NDNN, .35, .45, yaw_lr, lr},
    {NULL, NDNN, .45, .55, yaw_rr, rr},
    {NULL, NDNN, .55, .65, yaw_ls, ls},
    {NULL, NDNN, .65, .75, yaw_rs, rs},
    {NULL, NDNN, .75, 1., pitch_u, up},
    {NULL, NDNR, 0., .15, stop, sp},
    //{NULL, NDNR, , , forward_f, ff},
    {NULL, NDNR, .15, .3, forward_s, fss},
    {NULL, NDNR, .3, .7, yaw_lr, lr},
    {NULL, NDNR, .7, 1., yaw_ls, ls},
    {NULL, NDLN, 0., .15, stop, sp},
    //{NULL, NDLN, , , forward_f, ff},
    {NULL, NDLN, .15, .3, forward_s, fss},
    {NULL, NDLN, .3, .7, yaw_rr, rr},
    {NULL, NDLN, .7, 1., yaw_rs, rs},
    {NULL, NDLR, 0., .3, stop, sp},
    {NULL, NDLR, .3, .7, forward_s, fss},
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
    srand((int)robosharkstate.timestamp);
    float rand_prob = (rand() % 100) / 100.;
    BuffPrintf("RAND: %f\r\n", rand_prob);
//    float rand_period = rand() % 5 + 1.;
    for (i=0; i < pSM->transNum; i++)
    {
        if((pSM->transform[i].eventId == evt) && evt != NORMAL)
        {
            if((((pSM->transform[i].nextMotionState == yaw_lr) || (pSM->transform[i].nextMotionState == yaw_ls)) && robosharkstate.swim_param.motion_offset > 0.) &&
                (((pSM->transform[i].nextMotionState == yaw_rr) || (pSM->transform[i].nextMotionState == yaw_rs)) && robosharkstate.swim_param.motion_offset < 0.))
            {
                StateTransform tempTrans[] = {{pSM->state, evt, 1., 1., forward_s, fss}};     //当前状态不是NULL 表示是过渡情况
                return &tempTrans[0];
            }
            else if((evt == FNNN) && ((pSM->state == yaw_lr) || (pSM->state == yaw_ls)))
            {
                StateTransform tempTrans[] = {{pSM->state, evt, 0., 1., yaw_lr, lr}};     //当前状态不是NULL 表示是过渡情况
                return &tempTrans[0]; 
            }
            else if((evt == FNNN) && ((pSM->state == yaw_rr) || (pSM->state == yaw_rs)))
            {
                StateTransform tempTrans[] = {{pSM->state, evt, 0., 1., yaw_rr, rr}};     //当前状态不是NULL 表示是过渡情况
                return &tempTrans[0]; 
            }
            else if((rand_prob >= pSM->transform[i].transProb_d) && (rand_prob < pSM->transform[i].transProb_u))
            {
                return &pSM->transform[i];
            }
        }
        else if ((pSM->transform[i].eventId == evt) && (pSM->state == pSM->transform[i].curMotionState) && evt == NORMAL)
        {
            if((((pSM->transform[i].nextMotionState == yaw_lr) || (pSM->transform[i].nextMotionState == yaw_ls)) && robosharkstate.swim_param.motion_offset > 0.) &&
                (((pSM->transform[i].nextMotionState == yaw_rr) || (pSM->transform[i].nextMotionState == yaw_rs)) && robosharkstate.swim_param.motion_offset < 0.))
            {
                StateTransform tempTrans[] = {{pSM->state, evt, 1., 1., forward_s, fss}};   //当前状态不是NULL 表示是过渡情况
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
    if (AN_tmrcount < (int)(rand_period / 0.01) && (evt == last_evt))
    {
        AN_tmrcount++;
    }
    else
    {
        srand((int)robosharkstate.timestamp);
        StateTransform* pTrans; 
        MotionState LastState = pSM->state;
        pTrans = findTrans(pSM, evt);
        if(pTrans->transProb_d == 1.)
        {
            rand_period = 1; 
        }
        else
        {
            rand_period = rand() % 5 + 1; 
        }
        if (pTrans == NULL)
        {
            BuffPrintf("Current state %s NO SCHEDULE\r\n", dis_state[pSM->state-1]);
            return;
        }
        pSM->state = pTrans->nextMotionState;
        Action act = pTrans->action;
        if (act == NULL) 
        {
            BuffPrintf("State: %s --> %s, NO ACTION!\r\n", dis_state[LastState-1], dis_state[pSM->state-1]);
            return;
        }
        
        BuffPrintf("Period: %d s  CurEvt: %d, State: %s --> %s, Action: %s\r\n", rand_period, evt, 
                    dis_state[LastState-1], dis_state[pSM->state-1], dis_action[pTrans->action-1]);
        AN_tmrcount = 0;
        last_evt = evt;
    }
}

void auto_navigation_app_task()
{
    OS_ERR err;
    CPU_TS ts;

    StateMachine stateMachine;
    stateMachine.state = stop;
    stateMachine.transNum = (int) sizeof(stateTrans) / sizeof(stateTrans[0]);
    stateMachine.transform = stateTrans;
    while(1)
    {
        OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
        //OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);
        if (robosharkstate.infrared_data.obstacle_down_distance <= 50){
            obs_down = 0x01 << 2;
        }
        else{
            obs_down = 0x00;
        }
        obs_ahead = (1 - robosharkstate.infrared_data.obstacle_ahead) << 3;
        obs_left = (1 - robosharkstate.infrared_data.obstacle_left) << 1;
        obs_right = 1 - robosharkstate.infrared_data.obstacle_right;
        curEvt = (obs_ahead | obs_down | obs_left | obs_right) + 1;
        
        if (robosharkstate.autoctl_state == AutoCTL_RUN)
        {
            runStateMachine(&stateMachine, curEvt);
        }
        else
        {
            continue;
        }
    }
}

void auto_navigation_app_tmrcallback()
{
    OS_ERR err;
    
    OSTaskSemPost(&AutoNavigationTCB, OS_OPT_POST_NONE, &err);
}


// 初始化μCOS任务 auto_navigation
void auto_navigation_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&AutoNavigationTCB,
							 "Auto Navigation Task",
							 auto_navigation_app_task,
							 0,
							 AUTO_NAVIGATION_APP_TASK_PRIO, 
							 &AUTO_NAVIGATION_APP_TASK_STK[0],
							 AUTO_NAVIGATION_APP_STK_SIZE/10,
							 AUTO_NAVIGATION_APP_STK_SIZE,
							 0,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);

	OSTmrCreate(&AutoNavigationTmr,
							"Auto Navigation Timer",
							0,
							AUTO_NAVIGATION_APP_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)auto_navigation_app_tmrcallback,
							0,
							&err);
	
	OSTmrStart(&AutoNavigationTmr,
						&err);
							 
	OS_CRITICAL_EXIT();
}




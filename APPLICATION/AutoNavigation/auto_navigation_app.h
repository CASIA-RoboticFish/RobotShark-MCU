/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/

#ifndef _AUTO_NAVIGATION_H_
#define _AUTO_NAVIGATION_H_

#include "stdio.h"
#include "os_cfg_app.h"
#include "swim_control_app.h"
#include "stm32f4xx_conf.h"
#include "stdlib.h"
#include "robotstate.h"

#define AUTO_NAVIGATION_APP_TASK_PRIO 6  // 设置任务优先级,一般最少都有256个优先级，数字越小代表的优先级越高
#define AUTO_NAVIGATION_APP_STK_SIZE 256 // 设置任务堆栈大小（这里的1个代表4字节）
#define AUTO_NAVIGATION_APP_TIMER_PERIOD_MS 10 // 设置运行周期，这里说明期望这个任务每隔10ms运行一次
#define AUTO_NAVIGATION_APP_TIMER_PERIOD_TICKS (((uint32_t)AUTO_NAVIGATION_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ) /* 定时器时钟频率被设置为了 1000Hz */

// 记录前下左右四个传感器的探测距离
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30


// 状态机模式，该模式下，机器鱼的障碍检测分成16个事件 定义了可能出现的所有事件，根据四个传感器的返回值
typedef enum{
    NORMAL=1,       // 正常游动，周围没有检测到任何障碍
    NNNR,        
    NNLN,
    NNLR,
    NDNN,
    NDNR,
    NDLN,
    NDLR,
    FNNN,
    FNNR,
    FNLN,
    FNLR,
    FDNN,
    FDNR,
    FDLN,
    FDLR,
} EventID;

// 定义了7种运动状态：停止， 直游快、 直游慢、左急转、左缓转、右急转、右缓转、下潜、上浮
typedef enum{
    stop=1,
    forward_f,
    forward_s,
    yaw_lr,
    yaw_rr,
    yaw_ls,
    yaw_rs,
    pitch_u,
    pitch_d,
} MotionState;

typedef enum{
    sp=1,
    ff,
    fss,
    lr,
    rr,
    ls,
    rs,
    up,
    down,
} Action;

// 定义了一个状态机 包含 当前运动状态、突发事件、下一刻运动状态、下一运动状态下的执行函数
typedef struct{
    MotionState curMotionState;         // 当前的运动状态
    EventID eventId;                    // 事件id：16种 
    float transProb_d;              // 转换概率下限
    float transProb_u;              // 转换概率上限
    MotionState nextMotionState;        // 下一个运动状态
    Action action;                      // 下一个状态下要执行的动作
} StateTransform;

typedef struct{
    MotionState state;
    int transNum;
    StateTransform* transform;
} StateMachine;


extern EventID curEvt;

void runStateMachine(StateMachine* pSM, const EventID evt);
// 任务初始化函数
void auto_navigation_app_init(void);
// 任务暂停函数
void auto_navigation_app_stop(void);
// 任务恢复运行函数
void auto_navigation_app_resume(void);

#endif
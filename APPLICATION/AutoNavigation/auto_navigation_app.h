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

// 记录前下左右四个传感器的探测距离
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30

// 随机游走模式参数
#define     RS_time             5       // 正常状态下，随机游走的采样间隔上限，即控制动作改变周期最多为5秒
#define     RS_prob_s           0.1f    // 停止游动
#define     RS_prob_ff          0.2f    // 向前快速直游
#define     RS_prob_fs          0.3f    // 向前缓慢直游
#define     RS_prob_lr          0.35f   // 左急转
#define     RS_prob_ls          0.6f    // 左缓转
#define     RS_prob_rr          0.65f   // 右急转
#define     RS_prob_rs          0.9f    // 右缓转
#define     RS_prob_u           0.95f   // 下潜
#define     RS_prob_d           1.f     // 上浮


// 状态机模式，该模式下，机器鱼的障碍检测分成16个事件 定义了可能出现的所有事件，根据四个传感器的返回值
typedef enum{
    NORMAL = 0x00,       // 正常游动，周围没有检测到任何障碍
    NNNR = 0x01,        
    NNLN = 0x02,
    NNLR = 0x03,
    NDNN = 0x04,
    NDNR = 0x05,
    NDLN = 0x06,
    NDLR = 0x07,
    FNNN = 0x08,
    FNNR = 0x09,
    FNLN = 0x0A,
    FNLR = 0x0B,
    FDNN = 0x0C,
    FDNR = 0x0D,
    FDLN = 0x0E,
    FDLR = 0x0F,
} EventID;

// 定义了7种运动状态：停止， 直游快、 直游慢、左急转、左缓转、右急转、右缓转、下潜、上浮
typedef enum{
    stop = 0x00,
    forward_f,
    forward_s,
    yaw_lr,
    yaw_ls,
    yaw_rr,
    yaw_rs,
    pitch_u,
    pitch_d,
} MotionState;

typedef enum{
//    void (*turn_lr)(float, float);
//    void (*turn_ls)(float, float);
//    void (*turn_rr)(float, float);
//    void (*turn_rs)(float, float);
//    void (*go_s)(float, float);
//    void (*go_u)(float, uint8_t);
//    void (*go_d)(float, uint8_t);
    sp=0x00,
    ff,
    fs,
    lr,
    ls,
    rr,
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

void runStateMachine(StateMachine* pSM, const EventID evt);

#endif
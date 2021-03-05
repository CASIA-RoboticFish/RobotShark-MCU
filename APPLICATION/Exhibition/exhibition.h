/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/

#ifndef _EXHIBITION_H_
#define _EXHIBITION_H_

#include "stdio.h"
#include "os_cfg_app.h"

// 状态机模式，该模式下，机器鱼的障碍检测分成16个状态
// 名称定义四个位置分别为(Forward, Downward, Left, Right) 四个不同方向上分别进行检测障碍，如果检测到障碍就置1，没有就置0
#define     NORMAL              0x00        // 正常游动，周围没有检测到任何障碍
#define     NNNR                0x01        
#define     NNLN                0x02
#define     NNLR                0x03
#define     NDNN                0x04
#define     NDNR                0x05
#define     NDLN                0x06
#define     NDLR                0x07
#define     FNNN                0x08
#define     FNNR                0x09
#define     FNLN                0x0A
#define     FNLR                0x0B
#define     FDNN                0x0C
#define     FDNR                0x0D
#define     FDLN                0x0E
#define     FDLR                0x0F

// 记录前下左右四个传感器的探测距离
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30

// 随机游走模式参数
#define     RS_time             5       // 正常状态下，随机游走的采样间隔上限，即控制动作改变周期最多为5秒
#define     RS_prob_f           0.3f    // 
#define     RS_prob_lr          0.05f
#define     RS_prob_ls          0.25f
#define     RS_prob_rr          0.05f
#define     RS_prob_rs          0.25f
#define     RS_prob_d













#endif
/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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

// ��¼ǰ�������ĸ���������̽�����
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30

// �������ģʽ����
#define     RS_time             5       // ����״̬�£�������ߵĲ���������ޣ������ƶ����ı��������Ϊ5��
#define     RS_prob_s           0.1f    // ֹͣ�ζ�
#define     RS_prob_ff          0.2f    // ��ǰ����ֱ��
#define     RS_prob_fs          0.3f    // ��ǰ����ֱ��
#define     RS_prob_lr          0.35f   // ��ת
#define     RS_prob_ls          0.6f    // ��ת
#define     RS_prob_rr          0.65f   // �Ҽ�ת
#define     RS_prob_rs          0.9f    // �һ�ת
#define     RS_prob_u           0.95f   // ��Ǳ
#define     RS_prob_d           1.f     // �ϸ�


// ״̬��ģʽ����ģʽ�£���������ϰ����ֳ�16���¼� �����˿��ܳ��ֵ������¼��������ĸ��������ķ���ֵ
typedef enum{
    NORMAL = 0x00,       // �����ζ�����Χû�м�⵽�κ��ϰ�
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

// ������7���˶�״̬��ֹͣ�� ֱ�ο졢 ֱ��������ת����ת���Ҽ�ת���һ�ת����Ǳ���ϸ�
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

// ������һ��״̬�� ���� ��ǰ�˶�״̬��ͻ���¼�����һ���˶�״̬����һ�˶�״̬�µ�ִ�к���
typedef struct{
    MotionState curMotionState;         // ��ǰ���˶�״̬
    EventID eventId;                    // �¼�id��16�� 
    float transProb_d;              // ת����������
    float transProb_u;              // ת����������
    MotionState nextMotionState;        // ��һ���˶�״̬
    Action action;                      // ��һ��״̬��Ҫִ�еĶ���
} StateTransform;

typedef struct{
    MotionState state;
    int transNum;
    StateTransform* transform;
} StateMachine;

void runStateMachine(StateMachine* pSM, const EventID evt);

#endif
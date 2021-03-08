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

#define AUTO_NAVIGATION_APP_TASK_PRIO 6  // �����������ȼ�,һ�����ٶ���256�����ȼ�������ԽС��������ȼ�Խ��
#define AUTO_NAVIGATION_APP_STK_SIZE 256 // ���������ջ��С�������1������4�ֽڣ�
#define AUTO_NAVIGATION_APP_TIMER_PERIOD_MS 10 // �����������ڣ�����˵�������������ÿ��10ms����һ��
#define AUTO_NAVIGATION_APP_TIMER_PERIOD_TICKS (((uint32_t)AUTO_NAVIGATION_APP_TIMER_PERIOD_MS*1000)/OS_CFG_TMR_TASK_RATE_HZ) /* ��ʱ��ʱ��Ƶ�ʱ�����Ϊ�� 1000Hz */

// ��¼ǰ�������ĸ���������̽�����
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30


// ״̬��ģʽ����ģʽ�£���������ϰ����ֳ�16���¼� �����˿��ܳ��ֵ������¼��������ĸ��������ķ���ֵ
typedef enum{
    NORMAL=1,       // �����ζ�����Χû�м�⵽�κ��ϰ�
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

// ������7���˶�״̬��ֹͣ�� ֱ�ο졢 ֱ��������ת����ת���Ҽ�ת���һ�ת����Ǳ���ϸ�
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


extern EventID curEvt;

void runStateMachine(StateMachine* pSM, const EventID evt);
// �����ʼ������
void auto_navigation_app_init(void);
// ������ͣ����
void auto_navigation_app_stop(void);
// ����ָ����к���
void auto_navigation_app_resume(void);

#endif
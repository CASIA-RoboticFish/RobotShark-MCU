/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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

// ״̬��ģʽ����ģʽ�£���������ϰ����ֳ�16��״̬
// ���ƶ����ĸ�λ�÷ֱ�Ϊ(Forward, Downward, Left, Right) �ĸ���ͬ�����Ϸֱ���м���ϰ��������⵽�ϰ�����1��û�о���0
#define     NORMAL              0x00        // �����ζ�����Χû�м�⵽�κ��ϰ�
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

// ��¼ǰ�������ĸ���������̽�����
#define     DIST_CFG_F          80
#define     DIST_CFG_D          30
#define     DIST_CFG_LR         30

// �������ģʽ����
#define     RS_time             5       // ����״̬�£�������ߵĲ���������ޣ������ƶ����ı��������Ϊ5��
#define     RS_prob_f           0.3f    // 
#define     RS_prob_lr          0.05f
#define     RS_prob_ls          0.25f
#define     RS_prob_rr          0.05f
#define     RS_prob_rs          0.25f
#define     RS_prob_d













#endif
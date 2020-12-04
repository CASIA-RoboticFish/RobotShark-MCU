/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.28
History: 
*****************************************************************************/
#include "depth_control_app.h"

extern BOXFISH boxfishstate;

static OS_TMR DepthControlTmr; // ����һ����ʱ��
static OS_TCB DepthControlTCB; // ����һ��������ƿ�
__align(8) CPU_STK DEPTH_CONTROL_APP_TASK_STK[DEPTH_CONTROL_APP_STK_SIZE]; // ���������ջ

// ��ʱ���ص�������ÿ����ʱ����ͻ�ִ���������������������������ź������Ӷ���������
static void depth_control_app_tmrcallback(void)
{
	OS_ERR err;

	OSTaskSemPost(&DepthControlTCB, // �����ź�����DepthControl����
			OS_OPT_POST_NONE,
			&err);
}

// ���������������ǻᱻ����ִ�еĺ���
static void depth_control_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	uint8_t databuf[12];
	
	while(1)
	{
		// �ȴ��ź�����ÿ���ź����������ͼ�������ִ�У�����ͽ�������״̬
		OSTaskSemPend(0,
			      OS_OPT_PEND_BLOCKING,
			      &ts,
			      &err);
		
		// �����������
		// �����ɴ�����ɷ���
		databuf[0] = 0xAA;
		databuf[1] = 0xBB;
		databuf[2] = 0xCC;
		databuf[3] = 0xDD;
		databuf[4] = 0xEE;
		databuf[5] = 0xFF;
		RFLink_sendStruct(BOXFISH_5, SET_SWIM_RUN , databuf , 0);
	}
}

// �����ʼ������
void depth_control_app_init(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();
	
	// ��������
	OSTaskCreate(&DepthControlTCB, 		// ������ƿ�
		 "Depth Control App", 		// ��������
		 depth_control_app_task, 	// ����������
		 0,				// ���ݸ�����Ĳ���
		 DEPTH_CONTROL_APP_TASK_PRIO,   // �������ȼ�
		 &DEPTH_CONTROL_APP_TASK_STK[0], // �����ջ����ַ
		 DEPTH_CONTROL_APP_STK_SIZE/10,  // �����ջ�����λ
		 DEPTH_CONTROL_APP_STK_SIZE,     // �����ջ��С
		 0, // ���������ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
		 0, // ��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ���
		 0, // �û�����Ĵ洢��
		 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, // ������ض�ѡ��
							  // OS_OPT_TASK_NONE      ��ʾû���κ�ѡ�� 
							  // OS_OPT_TASK_STK_CHK   ָ���Ƿ������������Ķ�ջ 
							  // OS_OPT_TASK_STK_CLR   ָ���Ƿ����������Ķ�ջ 
							  // OS_OPT_TASK_SA VE_FP  ָ���Ƿ�洢����Ĵ�����CPU��Ҫ�и�������Ӳ��������ר�ô��뱣�渡��Ĵ����� 
		 &err);
	
	// ������ʱ��
	OSTmrCreate(&DepthControlTmr,    // ��ʱ��ָ��
		"Depth Control Timer",   // ��ʱ������
		0,                       // ��ʼ����ʱ�����ӳ�ֵ
		DEPTH_CONTROL_APP_TIMER_PERIOD_TICKS, // �ظ�����
		OS_OPT_TMR_PERIODIC, // ��ʱ������ѡ��, OS_OPT_TMR_ONE_SHOT�����ζ�ʱ����OS_OPT_TMR_PERIODIC�������ڶ�ʱ�� 
		(OS_TMR_CALLBACK_PTR)depth_control_app_tmrcallback, // ��ʱ���ص�����
		0, // �ص���������
		&err);

	OSTmrStart(&DepthControlTmr, &err); // ������ʱ��
	
	depth_control_app_stop();
							 
	OS_CRITICAL_EXIT();
}

// ��ͣ����
void depth_control_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTmrStop(&DepthControlTmr, // ��ͣ��ʱ��
		OS_OPT_TMR_NONE,
		0,
		&err);

	OSTaskSuspend(&DepthControlTCB, &err); // �������

	OS_CRITICAL_EXIT();
}

// �ָ�����
void depth_control_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&DepthControlTCB, &err); // ������ʱ��

	OSTmrStart(&DepthControlTmr, &err); // ��������

	OS_CRITICAL_EXIT();
}
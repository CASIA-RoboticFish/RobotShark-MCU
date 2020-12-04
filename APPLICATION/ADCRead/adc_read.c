/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "adc_read.h"

// UCOS��ر�������
OS_TCB ADCReadTaskTCB;
__align(8) CPU_STK ADC_READ_TASK_STK[ADC_READ_STK_SIZE];


// ����ȫ�ֱ���
u16 g_current_adcx = 0;


void adc_read_task(void *p_arg)
{
	OS_ERR err;
	
	float x_k_k1;
	float x_k1_k1 = 0;
	
	float Z_k;
	float P_k1_k1;

	float Q = 0.0003;
	float R = 10;
	float Kg = 0;
	float P_k_k1 = 1;

	u16 print_cnt = 0;
	while(1)
	{
		// ��ȡ��������
		Z_k = (float)(get_adc(ADC_Channel_5));
		
		// ʱ�����
		x_k_k1 = x_k1_k1;
    P_k_k1 = P_k1_k1 + Q;
		
		// �������
    Kg = P_k_k1/(P_k_k1 + R); // ���㿨��������
    x_k1_k1 = x_k_k1 + Kg * (Z_k - x_k_k1);
    P_k1_k1 = (1 - Kg)*P_k_k1;
		
		// ��ֵ
		g_current_adcx = (u16)x_k1_k1;
		//g_current_adcx = Z_k;
//		print_cnt++;
//		if(print_cnt==50)
//		{
//			BuffPrintf("FIL: %d \n", g_current_adcx);
//			print_cnt = 0;
//		}
		
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void adc_read_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&ADCReadTaskTCB,       //������ƿ�
							 (CPU_CHAR  * )"adc read task",         //��������
							 (OS_TASK_PTR )adc_read_task,             //������
							 (void      * )0,                   //���ݸ��������Ĳ���
							 (OS_PRIO     )ADC_READ_TASK_PRIO,     //�������ȼ�
							 (CPU_STK   * )&ADC_READ_TASK_STK[0],   //�����ջ����ַ
							 (CPU_STK_SIZE)ADC_READ_STK_SIZE/10,    //�����ջ�����λ
							 (CPU_STK_SIZE)ADC_READ_STK_SIZE,       //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ
	
  OS_CRITICAL_EXIT();
}
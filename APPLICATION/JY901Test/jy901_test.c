/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "jy901_test.h"

// UCOS��ر�������
OS_TCB JY901TestTaskTCB;
__align(8) CPU_STK JY901_TEST_TASK_STK[JY901_TEST_STK_SIZE];

// ����������
void jy901_test_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{

	}
}


void jy901_test_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&JY901TestTaskTCB,       //������ƿ�
							 (CPU_CHAR  * )"jy901 test task",         //��������
							 (OS_TASK_PTR )jy901_test_task,             //������
							 (void      * )0,                   //���ݸ��������Ĳ���
							 (OS_PRIO     )JY901_TEST_TASK_PRIO,     //�������ȼ�
							 (CPU_STK   * )&JY901_TEST_TASK_STK[0],   //�����ջ����ַ
							 (CPU_STK_SIZE)JY901_TEST_STK_SIZE/10,    //�����ջ�����λ
							 (CPU_STK_SIZE)JY901_TEST_STK_SIZE,       //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ
						 
	OS_CRITICAL_EXIT();
}
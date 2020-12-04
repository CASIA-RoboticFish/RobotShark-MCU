/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "jy901_test.h"

// UCOS相关变量定义
OS_TCB JY901TestTaskTCB;
__align(8) CPU_STK JY901_TEST_TASK_STK[JY901_TEST_STK_SIZE];

// 任务主函数
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
	
	OSTaskCreate((OS_TCB    * )&JY901TestTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"jy901 test task",         //任务名字
							 (OS_TASK_PTR )jy901_test_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )JY901_TEST_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&JY901_TEST_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)JY901_TEST_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)JY901_TEST_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
						 
	OS_CRITICAL_EXIT();
}
/********************************************************************************
* @File name: 
* @Author: Yang Xiang and Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "adc_read.h"

// UCOS相关变量定义
OS_TCB ADCReadTaskTCB;
__align(8) CPU_STK ADC_READ_TASK_STK[ADC_READ_STK_SIZE];


// 定义全局变量
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
		// 获取量测数据
		Z_k = (float)(get_adc(ADC_Channel_5));
		
		// 时间更新
		x_k_k1 = x_k1_k1;
    P_k_k1 = P_k1_k1 + Q;
		
		// 量测更新
    Kg = P_k_k1/(P_k_k1 + R); // 计算卡尔曼增益
    x_k1_k1 = x_k_k1 + Kg * (Z_k - x_k_k1);
    P_k1_k1 = (1 - Kg)*P_k_k1;
		
		// 赋值
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
	
	OSTaskCreate((OS_TCB    * )&ADCReadTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"adc read task",         //任务名字
							 (OS_TASK_PTR )adc_read_task,             //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )ADC_READ_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&ADC_READ_TASK_STK[0],   //任务堆栈基地址
							 (CPU_STK_SIZE)ADC_READ_STK_SIZE/10,    //任务堆栈深度限位
							 (CPU_STK_SIZE)ADC_READ_STK_SIZE,       //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	
  OS_CRITICAL_EXIT();
}
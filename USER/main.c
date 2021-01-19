/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: main.c
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "common.h"
#include "main.h"

// 初始化机器人结构体
BOXFISH boxfishstate = 
{
	0.0f,          // timestamp
	0, // 通讯基准时刻
	SWIM_INIT,     // swim_state
	{0.0f, 20.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // swim_param
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // imu_data
	0.0f // depth
};

// 初始化目标
ROBOTARGET robotarget = {0.0f, 0.0f};
/********************************************************************************
*
* Function name ：test_task
* Description   ：测试任务主函数
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void test_task(void *p_arg)
{
	OS_ERR err;
	OS_TICK tick;
	uint16_t ret = 0;
	float depth = 0.0f;
	while(1)
	{
//  	BuffPrintf("hello,world\n");
//		TIM_SetCompare2(TIM4, 1800);
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		TIM_SetCompare2(TIM4, 1500);
//		res = mf_open("message.txt", FA_READ);
//		if(res)
//			BuffPrintf("File open failed, error: %d\n", res);
//		mf_read(14);
//		mf_close();
//		BuffPrintf(fatbuf);
		//BuffPrintf("uart\n");
		//USART_SendData(USART3,0xff);
//		USART_SendData(USART1,0xEE);
//		TIM_SetCompare3(TIM4, 1800);
//		USART_SendData(USART6, 0xff);
//		led_on();
//		TIM_SetCompare4(TIM4, 1500);
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		TIM_SetCompare4(TIM4, 1000);
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		led_on();
//		depth = get_ms5837_data();
//		BuffPrintf("depth: %f\n", depth);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
		led_off();
//		depth = get_ms5837_data();
//		BuffPrintf("depth: %f\n", depth);
		//USART_SendData(USART3,0xdd);
//		USART_SendData(USART1,0xaa);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
	//	TIM_SetCompare3(TIM4, 1500);
//		USART_SendData(USART6, 0xee);
//		led_off();
	//	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		USART_SendData(USART6, 0xee);
	}
}

/********************************************************************************
*
* Function name ：start_task
* Description   ：开始任务主函数
*                 在这个任务中，其他的任务被陆续创建，最后开始任务被挂起
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
#if OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
     //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*1=1ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif
	CPU_Init(); // ???
	OS_CRITICAL_ENTER();
	
	// 创建BuffPrintf任务，优先级30
	BuffPrintf_Init();
	
	/*****************************************************************************/
	/*************************   Debug模式下的任务  *******************************/
	// 创建测试任务
	OSTaskCreate((OS_TCB    * )&TestTaskTCB,     	  //任务控制块
							 (CPU_CHAR  * )"test task",      	  //任务名字
							 (OS_TASK_PTR )test_task,          	//任务函数
							 (void      * )0,                 	//传递给任务函数的参数
							 (OS_PRIO     )TEST_TASK_PRIO,    		//任务优先级
							 (CPU_STK   * )&TEST_TASK_STK[0],	  //任务堆栈基地址
							 (CPU_STK_SIZE)TEST_STK_SIZE/10,  	  //任务堆栈深度限位
							 (CPU_STK_SIZE)TEST_STK_SIZE,    	  //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
																									//任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值								 
	
	/*****************************************************************************/
	// Application任务区，这部分任务是系统正常运行时将会启动的任务
	/*****************************************************************************/
	// 传感器数据刷新与存储
	sensors_update_app_init(); // 传感器数据更新，定时地访问传感器，将数据写入boxfishstate结构体中，优先级5
	data_storage_app_init();   // 数据储存（SD卡），将数据写入储存卡，优先级10
					 
	// 控制部分						 
	swim_control_app_init();     // 游动控制，基础的游动控制,优先级6
	// pantilt_control_task_init(); // 云台控制，这是我自己平台用到的程序，优先级13
	depth_control_app_init();    // 深度控制，优先级9
					 
	// 通讯部分						 
	command_analysis_app_init();       // 上位机通讯，优先级7
	command_analysis_slave_app_init(); // 树莓派通讯（或飞轮），优先级8
	command_analysis_friend_app_init();   // 水下多机器人分布式通讯，消息接收，优先级13	
	distributed_communicate_app_init(); // 水下多机器人分布式通讯，消息发布，优先级4						
//					 
	// 上位机部分，与上位机相关的程序
	data_sendback_app_init(); // 回传数据，优先级12
	data_show_app_init();     // 回传上位机曲线数据，优先级11
	/*****************************************************************************/
	/*****************************************************************************/	


						 
	/*****************************************************************************/	
	//挂起任务（测试任务）	，挂起就是不执行
	/*****************************************************************************/		
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); 
	OS_TaskSuspend((OS_TCB*)&TestTaskTCB,&err); 
	/*****************************************************************************/
	/*****************************************************************************/	

	OS_CRITICAL_EXIT(); //退出临界区
}

/********************************************************************************
*
* Function name ：main
* Description   ：初始化外设：串口，舵机，电机，电磁阀，水泵，压敏电阻，MPU9150
*                 初始化操作系统
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
int main(void)
{
	OS_ERR err; // 保存错误信息
	CPU_SR_ALLOC(); //宏，申明存储空间，用来存储当前CPU的中断状态
	
	// 系统初始化阶段
	delay_init(168); // 时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置了只用两位寄存器来指定中断的抢占式优先级
	
	// 以下是硬件初始化代码
	// LED初始化
	led_init();
	// 发送串口初始化
	uart3_init(19200);
	// 接收串口初始化
	uart2_init(9600);
	// 飞轮通信串口
	uart6_init(115200);
	// 水下分布式通信串口
	uart1_init(9600);
	// 胸鳍、尾鳍舵机初始化
	servo_init();
	// ADS1115初始化
	ads1115_init();
	// MS5837初始化
	ms5837_init();
	// SD卡初始化
	while(SD_Init())//检测不到SD卡
	{
		MyPrintf("SD Card Error!");
		delay_ms(500);					
		MyPrintf("Please Check! ");
		delay_ms(500);
	}
	// 初始化内部内存池 // 没有使用UCOS自带的，因为不好用
	my_mem_init(SRAMIN);
	// 文件系统初始化
	exfuns_init();
	mf_mount("0:",1);
	
	// IMU初始化 ,IMU不再采用MPU6050，而是采用JY901
	// JY901无需初始化，只需要等待一段时间，让其自动初始化完成	
	JY901_init();
	delay_ms(2000);
	
//#ifdef MPU_EXTERNAL	
//	// MPU9150初始化(1)——外接IMU
//	MPU_SetAddr(0x68);
//	mpu_set_address(0x68);
//	mpu_set_orientation(0x68);
//	delay_ms(2000);
//	if(MPU_Init()!=0)
//	{
//		MyPrintf("MPU9150 Init Error\n");
//		return 0;
//	}
//	while(mpu_dmp_init())
//	{
//		MyPrintf("MPU9150 DMP Init Error\n");
//		delay_ms(200);
//	}
//#endif

//#ifdef MPU_ONBOARD		
//	// MPU9150初始化(2)——板载IMU
//	MPU_SetAddr(0x69);
//	mpu_set_address(0x69);
//	mpu_set_orientation(0x69);
//	delay_ms(2000);
//	if(MPU_Init()!=0)
//	{
//		MyPrintf("MPU9150 Init Error\n");
//		return 0;
//	}
//	while(mpu_dmp_init())
//	{
//		MyPrintf("MPU9150 DMP Init Error\n");
//		delay_ms(200);
//	}
//#endif

	
	// 以下是操作系统初始化
	OSInit(&err);
	// 进入临界区
	OS_CRITICAL_ENTER();
	// 创建开始任务
	OSTaskCreate((OS_TCB    * )&StartTaskTCB,       //任务控制块
							 (CPU_CHAR  * )"start task",        //任务名字
							 (OS_TASK_PTR )start_task,          //任务函数
							 (void      * )0,                   //传递给任务函数的参数
							 (OS_PRIO     )START_TASK_PRIO,     //任务优先级
							 (CPU_STK   * )&START_TASK_STK[0],  //任务堆栈基地址
							 (CPU_STK_SIZE)START_STK_SIZE/10,   //任务堆栈深度限位
							 (CPU_STK_SIZE)START_STK_SIZE,      //任务堆栈大小
							 (OS_MSG_QTY  )0,                   //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK     )0,                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
							 (void      * )0,                   //用户补充的存储区
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
																									//任务选项
							 (OS_ERR    * )&err);               //存放该函数错误时的返回值
	// 退出临界区
	OS_CRITICAL_EXIT(); 
	// 开启UCOSIII
	OSStart(&err);  
	while(1);
}



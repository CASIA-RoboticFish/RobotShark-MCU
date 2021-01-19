/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: main.c
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "common.h"
#include "main.h"

// ��ʼ�������˽ṹ��
BOXFISH boxfishstate = 
{
	0.0f,          // timestamp
	0, // ͨѶ��׼ʱ��
	SWIM_INIT,     // swim_state
	{0.0f, 20.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // swim_param
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // imu_data
	0.0f // depth
};

// ��ʼ��Ŀ��
ROBOTARGET robotarget = {0.0f, 0.0f};
/********************************************************************************
*
* Function name ��test_task
* Description   ����������������
*                 
* Parameter     ��
* 		@null
* Return        ��null
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
* Function name ��start_task
* Description   ����ʼ����������
*                 ����������У�����������½�����������ʼ���񱻹���
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
#if OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
     //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*1=1ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif
	CPU_Init(); // ???
	OS_CRITICAL_ENTER();
	
	// ����BuffPrintf�������ȼ�30
	BuffPrintf_Init();
	
	/*****************************************************************************/
	/*************************   Debugģʽ�µ�����  *******************************/
	// ������������
	OSTaskCreate((OS_TCB    * )&TestTaskTCB,     	  //������ƿ�
							 (CPU_CHAR  * )"test task",      	  //��������
							 (OS_TASK_PTR )test_task,          	//������
							 (void      * )0,                 	//���ݸ��������Ĳ���
							 (OS_PRIO     )TEST_TASK_PRIO,    		//�������ȼ�
							 (CPU_STK   * )&TEST_TASK_STK[0],	  //�����ջ����ַ
							 (CPU_STK_SIZE)TEST_STK_SIZE/10,  	  //�����ջ�����λ
							 (CPU_STK_SIZE)TEST_STK_SIZE,    	  //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
																									//����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ								 
	
	/*****************************************************************************/
	// Application���������ⲿ��������ϵͳ��������ʱ��������������
	/*****************************************************************************/
	// ����������ˢ����洢
	sensors_update_app_init(); // ���������ݸ��£���ʱ�ط��ʴ�������������д��boxfishstate�ṹ���У����ȼ�5
	data_storage_app_init();   // ���ݴ��棨SD������������д�봢�濨�����ȼ�10
					 
	// ���Ʋ���						 
	swim_control_app_init();     // �ζ����ƣ��������ζ�����,���ȼ�6
	// pantilt_control_task_init(); // ��̨���ƣ��������Լ�ƽ̨�õ��ĳ������ȼ�13
	depth_control_app_init();    // ��ȿ��ƣ����ȼ�9
					 
	// ͨѶ����						 
	command_analysis_app_init();       // ��λ��ͨѶ�����ȼ�7
	command_analysis_slave_app_init(); // ��ݮ��ͨѶ������֣������ȼ�8
	command_analysis_friend_app_init();   // ˮ�¶�����˷ֲ�ʽͨѶ����Ϣ���գ����ȼ�13	
	distributed_communicate_app_init(); // ˮ�¶�����˷ֲ�ʽͨѶ����Ϣ���������ȼ�4						
//					 
	// ��λ�����֣�����λ����صĳ���
	data_sendback_app_init(); // �ش����ݣ����ȼ�12
	data_show_app_init();     // �ش���λ���������ݣ����ȼ�11
	/*****************************************************************************/
	/*****************************************************************************/	


						 
	/*****************************************************************************/	
	//�������񣨲�������	��������ǲ�ִ��
	/*****************************************************************************/		
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); 
	OS_TaskSuspend((OS_TCB*)&TestTaskTCB,&err); 
	/*****************************************************************************/
	/*****************************************************************************/	

	OS_CRITICAL_EXIT(); //�˳��ٽ���
}

/********************************************************************************
*
* Function name ��main
* Description   ����ʼ�����裺���ڣ�������������ŷ���ˮ�ã�ѹ�����裬MPU9150
*                 ��ʼ������ϵͳ
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
int main(void)
{
	OS_ERR err; // ���������Ϣ
	CPU_SR_ALLOC(); //�꣬�����洢�ռ䣬�����洢��ǰCPU���ж�״̬
	
	// ϵͳ��ʼ���׶�
	delay_init(168); // ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //������ֻ����λ�Ĵ�����ָ���жϵ���ռʽ���ȼ�
	
	// ������Ӳ����ʼ������
	// LED��ʼ��
	led_init();
	// ���ʹ��ڳ�ʼ��
	uart3_init(19200);
	// ���մ��ڳ�ʼ��
	uart2_init(9600);
	// ����ͨ�Ŵ���
	uart6_init(115200);
	// ˮ�·ֲ�ʽͨ�Ŵ���
	uart1_init(9600);
	// ������β�������ʼ��
	servo_init();
	// ADS1115��ʼ��
	ads1115_init();
	// MS5837��ʼ��
	ms5837_init();
	// SD����ʼ��
	while(SD_Init())//��ⲻ��SD��
	{
		MyPrintf("SD Card Error!");
		delay_ms(500);					
		MyPrintf("Please Check! ");
		delay_ms(500);
	}
	// ��ʼ���ڲ��ڴ�� // û��ʹ��UCOS�Դ��ģ���Ϊ������
	my_mem_init(SRAMIN);
	// �ļ�ϵͳ��ʼ��
	exfuns_init();
	mf_mount("0:",1);
	
	// IMU��ʼ�� ,IMU���ٲ���MPU6050�����ǲ���JY901
	// JY901�����ʼ����ֻ��Ҫ�ȴ�һ��ʱ�䣬�����Զ���ʼ�����	
	JY901_init();
	delay_ms(2000);
	
//#ifdef MPU_EXTERNAL	
//	// MPU9150��ʼ��(1)�������IMU
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
//	// MPU9150��ʼ��(2)��������IMU
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

	
	// �����ǲ���ϵͳ��ʼ��
	OSInit(&err);
	// �����ٽ���
	OS_CRITICAL_ENTER();
	// ������ʼ����
	OSTaskCreate((OS_TCB    * )&StartTaskTCB,       //������ƿ�
							 (CPU_CHAR  * )"start task",        //��������
							 (OS_TASK_PTR )start_task,          //������
							 (void      * )0,                   //���ݸ��������Ĳ���
							 (OS_PRIO     )START_TASK_PRIO,     //�������ȼ�
							 (CPU_STK   * )&START_TASK_STK[0],  //�����ջ����ַ
							 (CPU_STK_SIZE)START_STK_SIZE/10,   //�����ջ�����λ
							 (CPU_STK_SIZE)START_STK_SIZE,      //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
																									//����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ
	// �˳��ٽ���
	OS_CRITICAL_EXIT(); 
	// ����UCOSIII
	OSStart(&err);  
	while(1);
}



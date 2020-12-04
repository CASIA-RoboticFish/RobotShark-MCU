/********************************************************************************
* @File name: 
* @Author: Vincent Fei
* @Version: 1.1
* @Date: 2018-12-1
* @Description: 
********************************************************************************/
#include "servo_disturb.h"

// UCOS��ر�������
OS_TCB RollServoDisturbTaskTCB;
__align(8) CPU_STK ROLL_SERVO_DISTURB_TASK_STK[ROLL_SERVO_DISTURB_STK_SIZE];
static OS_TMR RollServoTmr;

OS_TCB YawServoDisturbTaskTCB;
__align(8) CPU_STK YAW_SERVO_DISTURB_TASK_STK[YAW_SERVO_DISTURB_STK_SIZE];
static OS_TMR YawServoTmr;


static void roll_servo_tmrcallBack ( void )
{
    OS_ERR err;

    OSTaskSemPost(&RollServoDisturbTaskTCB,
                  OS_OPT_POST_NONE,
                  &err);    
}	

static void yaw_servo_tmrcallBack ( void )
{
    OS_ERR err;

    OSTaskSemPost(&YawServoDisturbTaskTCB,
                  OS_OPT_POST_NONE,
                  &err);    
}	

// ����������
void roll_servo_disturb_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK tick;
	float t = 0.0f;
	
	SERVO_CH servo_channel;
	servo_channel = SERVO_1;
	float pi = 3.1415926;
	float angle_interval = 0.0;
	float servo1_rotate_amplitude = 40;    // ��λ����
	float servo1_rotate_speed = 3.1667;    // ��λ����ÿ����
	float servo1_expect_frequency = 0.5;     // ��λ��Hz
	float servo1_current_time = 0;
	float servo1_current_angle = 0;
	uint32_t servo1_time_interval = 1;
	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		tick = OSTimeGet(&err);
    t = (float)tick / (float)OS_CFG_TICK_RATE_HZ;
		servo1_current_angle = servo1_rotate_amplitude*sin(2*pi*servo1_expect_frequency*t);
		pwm_set_angle(servo1_current_angle,servo_channel);
		
	}
}

// ����������
void yaw_servo_disturb_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_TICK tick;
	float t = 0.0f;
	
	SERVO_CH servo_channel;
	servo_channel = SERVO_2;
	float pi = 3.1415926;
	float angle_interval = 0.0;
	//float servo2_rotate_amplitude = 40;    // ��λ���� //���60��
	float servo2_rotate_amplitude = 22;
	float servo2_rotate_speed = 3.1667;    // ��λ����ÿ����
	float servo2_expect_frequency = 0.5;     // ��λ��Hz
	float servo2_current_time = 0;
	float servo2_current_angle = 0;
	uint32_t servo2_time_interval = 10;
	
	
	while(1)
	{
		OSTaskSemPend(0,
									OS_OPT_PEND_BLOCKING,
									&ts,
									&err);
		
		tick = OSTimeGet(&err);
    t = (float)tick / (float)OS_CFG_TICK_RATE_HZ;
		servo2_current_angle = servo2_rotate_amplitude*sin(2*pi*servo2_expect_frequency*t);
		pwm_set_angle(servo2_current_angle,servo_channel);
	}
}


// �����Ŷ������ת����
void roll_servo_disturb_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&RollServoDisturbTaskTCB,       //������ƿ�
							 (CPU_CHAR  * )"roll servo disturb control task",         //��������
							 (OS_TASK_PTR )roll_servo_disturb_task,             //������
							 (void      * )0,                   //���ݸ��������Ĳ���
							 (OS_PRIO     )ROLL_SERVO_DISTURB_TASK_PRIO,     //�������ȼ�
							 (CPU_STK   * )&ROLL_SERVO_DISTURB_TASK_STK[0],   //�����ջ����ַ
							 (CPU_STK_SIZE)ROLL_SERVO_DISTURB_STK_SIZE/10,    //�����ջ�����λ
							 (CPU_STK_SIZE)ROLL_SERVO_DISTURB_STK_SIZE,       //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ

	OSTmrCreate(&RollServoTmr,
							"roll control timer",
							0,
							ROLL_SERVO_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)roll_servo_tmrcallBack,
							0,
							&err);
	
	// ������ʱ��
	OSTmrStart(&RollServoTmr,
             &err);
							 
	OS_CRITICAL_EXIT();
}

// �����Ŷ������ת����
void yaw_servo_disturb_task_init()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB    * )&YawServoDisturbTaskTCB,       //������ƿ�
							 (CPU_CHAR  * )"yaw servo disturb control task",         //��������
							 (OS_TASK_PTR )yaw_servo_disturb_task,             //������
							 (void      * )0,                   //���ݸ��������Ĳ���
							 (OS_PRIO     )YAW_SERVO_DISTURB_TASK_PRIO,     //�������ȼ�
							 (CPU_STK   * )&YAW_SERVO_DISTURB_TASK_STK[0],   //�����ջ����ַ
							 (CPU_STK_SIZE)YAW_SERVO_DISTURB_STK_SIZE/10,    //�����ջ�����λ
							 (CPU_STK_SIZE)YAW_SERVO_DISTURB_STK_SIZE,       //�����ջ��С
							 (OS_MSG_QTY  )0,                   //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK     )0,                   //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
							 (void      * )0,                   //�û�����Ĵ洢��
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
							 (OS_ERR    * )&err);               //��Ÿú�������ʱ�ķ���ֵ

	 OSTmrCreate(&YawServoTmr,
							"gimbal control timer",
							0,
							YAW_SERVO_TIMER_PERIOD_TICKS,
							OS_OPT_TMR_PERIODIC,
							(OS_TMR_CALLBACK_PTR)yaw_servo_tmrcallBack,
							0,
							&err);
	
	// ������ʱ��
	OSTmrStart(&YawServoTmr,
             &err);
							 
	OS_CRITICAL_EXIT();
}
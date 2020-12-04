/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "motor.h"

/********************************************************************************
*
* Function name ��TB6612FNG1_init
* Description   ����ʼ�����Ƶ�һ��TB6612FNG����Ķ�ʱ����GPIO��
*                 �������õ�PWMƵ��Ϊ1KHz��PWM�����ֵΪ1000
* Parameter     ��
* 		@arr
*     @psc
* Return        ��null
********************************************************************************/
static void TB6612FNG1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	// PE0:MOTOR1_IN1
	// PE1:MOTOR1_IN2
	// PE2:MOTOR2_IN1
	// PE3:MOTOR2_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_2 | GPIO_Pin_3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

	//ʹ��GPIOD��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	// PD0:MOTOR12_STBY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// ʹ��GPIOB��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// PB4:MOTOR1_PWM
	// PB5:MOTOR2_PWM
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ʹ�ø��ù������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ʹ���������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ��ʹ������������
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

	// ����Ϊ���ù��ܣ�ָ������ĸ��ù��ܣ�
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);


	// ʹ��TIM3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// ���������ÿ����ã�pwm��Ƶ��Ϊ1KHz����Ϊʱ��Ƶ����84MHz��
	// ��Ϊ84MHz/84/1000=1KHz�����⣬pwm���ֻ���赽1000��
	// TIM3��ʱ��Ƶ����84MHz
  // �趨ʱ��
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// ��ʼ�����ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	// ʹ�����ͨ������
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	// ʹ�����ؼĴ���
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// ʹ�ܶ�ʱ������ʱ����ʼ������
	TIM_Cmd(TIM3, ENABLE);

	// ʹ��PWM���
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

/********************************************************************************
*
* Function name ��TB6612FNG2_init
* Description   ����ʼ�����Ƶڶ���TB6612FNG����Ķ�ʱ����GPIO��
*                 �������õ�PWMƵ��Ϊ1KHz��PWM�����ֵΪ1000
* Parameter     ��
* 		@arr
*     @psc
* Return        ��null
********************************************************************************/
static void TB6612FNG2_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	// PA6:MOTOR3_IN1
	// PA7:MOTOR3_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
	
	//ʹ��GPIOC��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	// PC4:MOTOR4_IN1
	// PC5:MOTOR4_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5);
	
	//ʹ��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	// PE12:MOTOR34_STBY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// ʹ��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	// PE9:MOTOR3_PWM
	// PE11:MOTOR4_PWM
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_11);    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ʹ�ø��ù������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ʹ���������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ��ʹ������������
	GPIO_Init(GPIOE, &GPIO_InitStructure);    

	// ����Ϊ���ù��ܣ�ָ������ĸ��ù��ܣ�
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);


	// ʹ��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	// ���������ÿ����ã�pwm��Ƶ��Ϊ1KHz����Ϊʱ��Ƶ����84MHz��
	// ��Ϊ84MHz/84/1000=1KHz�����⣬pwm���ֻ���赽1000��
	// TIM1��ʱ��Ƶ����84MHz
  // �趨ʱ��
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	// ��ʼ�����ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	// ʹ�����ͨ������
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	// ʹ�����ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	// ʹ�ܶ�ʱ������ʱ����ʼ������
	TIM_Cmd(TIM1, ENABLE);

	// ʹ��PWM���
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/********************************************************************************
*
* Function name ��motor_startup
* Description   ���������
*                 
* Parameter     ��
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_startup(uint8_t channel)
{
	switch(channel)
	{
		case MOTOR1:
			GPIO_SetBits(GPIOD, GPIO_Pin_0);
			break;
		case MOTOR2:
			GPIO_SetBits(GPIOD, GPIO_Pin_0);
			break;
		case MOTOR3:
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
			break;
		case MOTOR4:
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��motor_standby
* Description   �����׼��
*                 
* Parameter     ��
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_standby(uint8_t channel)
{
	switch(channel)
	{
		case MOTOR1:
			GPIO_ResetBits(GPIOD, GPIO_Pin_0);
			break;
		case MOTOR2:
			GPIO_ResetBits(GPIOD, GPIO_Pin_0);
			break;
		case MOTOR3:
			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
			break;
		case MOTOR4:
			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��motor_stop
* Description   �����ֹͣ
*                 
* Parameter     ��
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_stop(uint8_t channel)
{
	switch(channel)
	{
		case MOTOR1:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
			break;
		case MOTOR2:
			GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3);
			break;
		case MOTOR3:
			GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
			break;
		case MOTOR4:
			GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��motor_forward_rotate
* Description   �������ת
*                 
* Parameter     ��
*     @compare_value ���������PWM�źŵ�ռ�ձ�
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_forward_rotate(uint16_t compare_value, uint8_t channel)
{
	
	switch(channel)
	{
		case MOTOR1:
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			TIM_SetCompare1(TIM3, compare_value);
			break;
		case MOTOR2:
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			TIM_SetCompare2(TIM3, compare_value);
			break;
		case MOTOR3:
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			TIM_SetCompare1(TIM1, compare_value);
			break;
		case MOTOR4:
			GPIO_SetBits(GPIOC, GPIO_Pin_4);
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
			TIM_SetCompare2(TIM1, compare_value);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��motor_backward_rotate
* Description   �������ת
*                 
* Parameter     ��
*     @compare_value ���������PWM�źŵ�ռ�ձ�
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_backward_rotate(uint16_t compare_value, uint8_t channel)
{
	
	switch(channel)
	{
		case MOTOR1:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			TIM_SetCompare1(TIM3, compare_value);
			break;
		case MOTOR2:
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			TIM_SetCompare2(TIM3, compare_value);
			break;
		case MOTOR3:
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
			GPIO_SetBits(GPIOA, GPIO_Pin_7);
			TIM_SetCompare1(TIM1, compare_value);
			break;
		case MOTOR4:
			GPIO_ResetBits(GPIOC, GPIO_Pin_4);
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
			TIM_SetCompare2(TIM1, compare_value);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��motor_rotate
* Description   �������ת
*                 
* Parameter     ��
*     @compare_value ���������PWM�źŵ�ռ�ձ�
* 		@channel  �����ͨ����������·ͨ��   
* Return        ��null
********************************************************************************/
void motor_rotate(int compare_value, uint8_t channel)
{	
	if(compare_value > PWM_MAX_VALUE)
	{
		motor_forward_rotate((uint16_t)PWM_MAX_VALUE,channel);
	}
	else if(compare_value > 0)
	{
		motor_forward_rotate((uint16_t)compare_value,channel);
	}
	else if(compare_value == 0)
	{
		motor_stop(channel);
	}
	else if(compare_value > -PWM_MAX_VALUE)
	{
		motor_backward_rotate((uint16_t)(-compare_value),channel);
	}
	else
	{
		motor_backward_rotate((uint16_t)(PWM_MAX_VALUE),channel);
	}
}
	
/********************************************************************************
*
* Function name ��motor_init
* Description   �������ʼ��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/	
void motor_init(void)
{
	TB6612FNG1_init();
	TB6612FNG2_init();
	
	motor_startup(MOTOR1);
	motor_stop(MOTOR1);
	
	motor_startup(MOTOR2);
	motor_stop(MOTOR2);
	
	motor_startup(MOTOR3);
	motor_stop(MOTOR3);
	
	motor_startup(MOTOR4);
	motor_stop(MOTOR4);
}


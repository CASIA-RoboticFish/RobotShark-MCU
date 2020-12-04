/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "pectoral_servo_pwm.h"

/********************************************************************************
*
* Function name ��pwm_tim3_init
* Description   ����ʼ����ʱ���������������������PWM�ź�
*                 
* Parameter     ��
* 		@arr
*     @psc     
* Return        ��null
********************************************************************************/
static void pwm_tim3_init(uint32_t arr, uint32_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // PWM����ʹ������
    // TIM3C3 --> PB0
    // TIM3C4 --> PB1
    // ʹ��GPIOB��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_0 | GPIO_Pin_1 ); 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ʹ�ø��ù������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz��������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ʹ���������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ��ʹ������������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

		// ����Ϊ���ù��ܣ�ָ������ĸ��ù��ܣ�
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);    
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    // �趨ʱ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


    // �趨���ͨ��������ʹ�õ���TIM4��C1ͨ����
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		//TIM_OCInitStructure.TIM_Pulse = 1000; //������õ�ò���ǳ�ʼʱ���λ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // ʹ�����ͨ��һ�����ؼĴ���

		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
    // ʹ��TIM4��ARR����
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    // ʹ��TIM4����ʹ�ö�ʱ����ʼ����
    TIM_Cmd(TIM3, ENABLE);

    // ʹ��TIM4��PWM�����ֻ�������������������PWM����
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}		/* -----  end of function PWM_TIM4Init  ----- */

/********************************************************************************
*
* Function name ��pectoral_pwm_set_compare
* Description   �����������ʼ��
*                 
* Parameter     ��
* 		@compare_value �����PWM�Ĵ�С
*     @channel  �����ͨ��������·
* Return        ��null
********************************************************************************/
void pectoral_pwm_set_compare(uint16_t compare_value, uint8_t channel)
{
	switch(channel)
	{
		case PECTORAL_SERVO1:
			TIM_SetCompare3(TIM3, compare_value);
			break;
		case PECTORAL_SERVO2:
			TIM_SetCompare4(TIM3, compare_value);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��pectoral_servo_init
* Description   �����������ʼ��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void pectoral_servo_init()
{
	// ���������ÿ����ã�pwm��Ƶ��Ϊ50Hz����Ϊʱ��Ƶ����84MHz��
	// ��Ϊ84MHz/84/20000=50Hz�����⣬pwm���ֻ���赽20000�����Ƕ��ڶ������900~2100
	// һ��������20ms�����һ��ߵ�ƽ��0.9ms��2.1ms
	pwm_tim3_init(20000, 84);
	TIM_SetCompare3(TIM3, PECTORAL_SERVO1_MID_VALUE);
	TIM_SetCompare4(TIM3, PECTORAL_SERVO2_MID_VALUE);
}
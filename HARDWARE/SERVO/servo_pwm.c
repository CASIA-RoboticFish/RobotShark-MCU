/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/

#include "servo_pwm.h"
/********************************************************************************
*
* Function name ��pwm_tim4_init
* Description   ����ʼ����ʱ������������β�������PWM�ź�
*                 
* Parameter     ��
* 		@arr
*     @psc     
* Return        ��null
********************************************************************************/
static void pwm_tim4_init ( uint32_t arr, uint32_t psc )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // PWM����ʹ������
    // TIM4C3 --> PB8
    // TIM4C4 --> PC9
    // ʹ��GPIOB��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    
    
		GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ); 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ʹ�ø��ù������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz��������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ʹ���������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ��ʹ������������
    GPIO_Init(GPIOD, &GPIO_InitStructure);

		// ����Ϊ���ù��ܣ�ָ������ĸ��ù��ܣ�
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);    
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    // �趨ʱ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


    // �趨���ͨ��������ʹ�õ���TIM4��C1ͨ����
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		//TIM_OCInitStructure.TIM_Pulse = 1000; //������õ�ò���ǳ�ʼʱ���λ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);

    // ʹ�����ͨ��һ�����ؼĴ���
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
    // ʹ��TIM4��ARR����
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    // ʹ��TIM4����ʹ�ö�ʱ����ʼ����
    TIM_Cmd(TIM4, ENABLE);

    // ʹ��TIM4��PWM�����ֻ�������������������PWM����
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
}		/* -----  end of function PWM_TIM4Init  ----- */


/********************************************************************************
*
* Function name ��servo_pwm_set_compare
* Description   �������ʼ��
*                 
* Parameter     ��
* 		@compare_value �����PWM�Ĵ�С
*     @channel  �����ͨ��������·
* Return        ��null
********************************************************************************/
void servo_pwm_set_compare( uint16_t compare_value, uint8_t channel )
{
	switch(channel)
	{
		case SERVO1:
			TIM_SetCompare1(TIM4, compare_value);
			break;
		case SERVO2:
			TIM_SetCompare2(TIM4, compare_value);
			break;
		case SERVO3:
			TIM_SetCompare3(TIM4, compare_value);
			break;
		case SERVO4:
			TIM_SetCompare4(TIM4, compare_value);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ��servo_init
* Description   �������ʼ��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void servo_init ( void )
{
	// ���������ÿ����ã�pwm��Ƶ��Ϊ50Hz����Ϊʱ��Ƶ����84MHz��
	// ��Ϊ84MHz/84/20000=50Hz�����⣬pwm���ֻ���赽20000�����Ƕ��ڶ������900~2100
	// һ��������20ms�����һ��ߵ�ƽ��0.9ms��2.1ms
  pwm_tim4_init(20000, 84);
	servo_pwm_set_compare(SERVO1_MID_VALUE,SERVO1);
	servo_pwm_set_compare(SERVO2_MID_VALUE,SERVO2);
	servo_pwm_set_compare(SERVO3_MID_VALUE,SERVO3);
	servo_pwm_set_compare(SERVO4_MID_VALUE,SERVO4);
}

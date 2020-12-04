/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "pump.h"

/********************************************************************************
*
* Function name ��pwm_tim1_init
* Description   ����ʼ����ʱ������������ˮ�õ�PWM�ź�
*                 
* Parameter     ��
* 		@arr
*     @psc
* Return        ��null
********************************************************************************/
static void pwm_tim1_init(uint32_t arr, uint32_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0}; 

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // ʹ��GPIOA��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_11 );           // ����7������6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ʹ�ø��ù������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz��������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ʹ���������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ��ʹ������������
		
    GPIO_Init(GPIOA, &GPIO_InitStructure);    

    // ����Ϊ���ù��ܣ�ָ������ĸ��ù��ܣ�
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);


    // ʹ��TIM1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // TIM3��ʱ��Ƶ����84MHz
	// �趨ʱ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ��ʼ�����ͨ��
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		

    // ʹ�����ͨ������
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    // ʹ�����ؼĴ���
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    // ʹ�ܶ�ʱ������ʱ����ʼ������
    TIM_Cmd(TIM1, ENABLE);

    // ʹ��PWM���
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}		/* -----  end of function PWM_TIM3Init  ----- */

/********************************************************************************
*
* Function name ��pump_init
* Description   ��ˮ�ó�ʼ��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void pump_init(void)
{
	// ���������ÿ����ã�pwm��Ƶ��Ϊ1KHz����Ϊʱ��Ƶ����84MHz��
	// ��Ϊ84MHz/84/1000=1KHz�����⣬pwm���ֻ���赽1000��
	pwm_tim1_init(1000,84); 
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 0);
}


/********************************************************************************
*
* Function name ��pump_on
* Description   ����ˮ��
*                 �������ǧ��Ҫע�⣬�����װ�ˮ���ջ���ˮ�þ��Բ��ܷ�ת
*                 TIM_SetCompare4������0
*                 PUMP_PWM_OUTPUT������������Ϊ���ã���������1000
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void pump_on(void)
{
	TIM_SetCompare3(TIM1, PUMP_PWM_OUTPUT);
	TIM_SetCompare4(TIM1, 0);
}

/********************************************************************************
*
* Function name ��pump_off
* Description   ���ر�ˮ��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void pump_off(void)
{
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 0);
}



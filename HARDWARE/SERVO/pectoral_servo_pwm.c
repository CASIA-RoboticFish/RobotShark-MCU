/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
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
* Function name ：pwm_tim3_init
* Description   ：初始化定时器，产生控制胸鳍舵机的PWM信号
*                 
* Parameter     ：
* 		@arr
*     @psc     
* Return        ：null
********************************************************************************/
static void pwm_tim3_init(uint32_t arr, uint32_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // PWM引脚使用配置
    // TIM3C3 --> PB0
    // TIM3C4 --> PB1
    // 使能GPIOB的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_0 | GPIO_Pin_1 ); 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 使用复用功能输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz高速引脚
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 使用推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 不使用上下拉电阻
    GPIO_Init(GPIOB, &GPIO_InitStructure);

		// 设置为复用功能（指定具体的复用功能）
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);    
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    // 设定时基
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


    // 设定输出通道（我们使用的是TIM4的C1通道）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		//TIM_OCInitStructure.TIM_Pulse = 1000; //这个设置的貌似是初始时候的位置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // 使能输出通道一的重载寄存器

		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
    // 使能TIM4的ARR重载
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    // 使能TIM4，即使得定时器开始工作
    TIM_Cmd(TIM3, ENABLE);

    // 使能TIM4的PWM输出，只有这样才能真正的输出PWM波形
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}		/* -----  end of function PWM_TIM4Init  ----- */

/********************************************************************************
*
* Function name ：pectoral_pwm_set_compare
* Description   ：胸鳍舵机初始化
*                 
* Parameter     ：
* 		@compare_value ：舵机PWM的大小
*     @channel  ：舵机通道，共两路
* Return        ：null
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
* Function name ：pectoral_servo_init
* Description   ：胸鳍舵机初始化
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void pectoral_servo_init()
{
	// 这样的设置可以让，pwm的频率为50Hz，因为时钟频率是84MHz。
	// 因为84MHz/84/20000=50Hz。另外，pwm最大只能设到20000。但是对于舵机就是900~2100
	// 一个周期是20ms，舵机一般高电平是0.9ms到2.1ms
	pwm_tim3_init(20000, 84);
	TIM_SetCompare3(TIM3, PECTORAL_SERVO1_MID_VALUE);
	TIM_SetCompare4(TIM3, PECTORAL_SERVO2_MID_VALUE);
}
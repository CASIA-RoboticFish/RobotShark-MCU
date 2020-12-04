/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
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
* Function name ：pwm_tim1_init
* Description   ：初始化定时器，产生控制水泵的PWM信号
*                 
* Parameter     ：
* 		@arr
*     @psc
* Return        ：null
********************************************************************************/
static void pwm_tim1_init(uint32_t arr, uint32_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	// PE10:MOTOR1_ENB
	// PE12:MOTOR2_ENB
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_12);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// 使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	// PE9:MOTOR1_PWM1
	// PE11:MOTOR1_PWM2
	// PE13:MOTOR2_PWM1
	// PE14:MOTOR2_PWM2	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14);    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 使用复用功能输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz高速引脚
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 使用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 不使用上下拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure);    

	// 设置为复用功能（指定具体的复用功能）
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

	// 使能TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	// 这样的设置可以让，pwm的频率为1KHz，因为时钟频率是84MHz。
	// 因为84MHz/84/1000=1KHz。另外，pwm最大只能设到1000。
	// TIM1的时钟频率是84MHz
  // 设定时基
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	// 初始化输出通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	// 使能输出通道重载
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	// 使能重载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	// 使能定时器（定时器开始计数）
	TIM_Cmd(TIM1, ENABLE);

	// 使能PWM输出
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/********************************************************************************
*
* Function name ：pump_init
* Description   ：水泵初始化
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void pump_init(void)
{
	// 这样的设置可以让，pwm的频率为1KHz，因为时钟频率是84MHz。
	// 因为84MHz/84/1000=1KHz。另外，pwm最大只能设到1000。
	pwm_tim1_init(1000,84); 
	TIM_SetCompare1(TIM1, 0);
	TIM_SetCompare2(TIM1, 0);
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 0);
}


/********************************************************************************
*
* Function name ：pump_on
* Description   ：打开水泵
*                 这个函数千万要注意，很容易把水泵烧坏，水泵绝对不能反转
*                 TIM_SetCompare2和TIM_SetCompare4必须是0
*                 PUMP_PWM_OUTPUT参数，可以人为设置，但最大就是1000
*                 两个通道，PUMP_IN是吸水的，PUMP_OUT是抽水的
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void pump_on(uint8_t channel)
{
	if(channel == PUMP_IN)
	{
		TIM_SetCompare1(TIM1, PUMP_PWM_OUTPUT);
		TIM_SetCompare2(TIM1, 0);
	}
	else if(channel == PUMP_OUT)
	{
		TIM_SetCompare3(TIM1, PUMP_PWM_OUTPUT);
		TIM_SetCompare4(TIM1, 0);
	}
}

/********************************************************************************
*
* Function name ：pump_off
* Description   ：关闭水泵
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void pump_off(uint8_t channel)
{
	if(channel == PUMP_IN)
	{
		TIM_SetCompare1(TIM1, 0);
		TIM_SetCompare2(TIM1, 0);
	}
	else if(channel == PUMP_OUT)
	{
		TIM_SetCompare3(TIM1, 0);
		TIM_SetCompare4(TIM1, 0);
	}
}



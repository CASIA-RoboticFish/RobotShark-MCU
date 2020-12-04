/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "reaction_wheel.h"

/********************************************************************************
*
* Function name ：rw_encoder_init
* Description   ：飞轮编码器初始化，用定时器的编码器模式
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void rw_encoder_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);    
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_DeInit(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	//TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);
}

/********************************************************************************
*
* Function name ：rw_motor_init
* Description   ：飞轮电机初始化
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void rw_motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// 使能GPIOA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_11 );           // 引脚7和引脚6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 使用复用功能输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz高速引脚
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 使用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    // 不使用上下拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	// 设置为复用功能（指定具体的复用功能）
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);


	// 使能TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	// TIM3的时钟频率是84MHz
	// 设定时基
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // pwm 1kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	// 初始化输出通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	

	// 使能输出通道重载
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	// 使能重载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	// 使能定时器（定时器开始计数）
	TIM_Cmd(TIM1, ENABLE);

	// 使能PWM输出
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	/* -----  end of function PWM_TIM3Init  ----- */
	TIM_SetCompare3(TIM1, 0);
}

/********************************************************************************
*
* Function name ：rw_read_encoder
* Description   ：读取飞轮编码器读数
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
int rw_read_encoder(void)
{
	int encoder_num;
	encoder_num = (int)((int16_t)(TIM3->CNT));
	TIM_SetCounter(TIM3, 0);
	return encoder_num;
}

/********************************************************************************
*
* Function name ：rw_motor_speed
* Description   ：控制飞轮电机转速
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void rw_motor_speed(u16 pwm)
{
	if(pwm>300)
		pwm = 300;
	else if(pwm<0)
		pwm = 0;
	TIM_SetCompare3(TIM1, pwm);
}


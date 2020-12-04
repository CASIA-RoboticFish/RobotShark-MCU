/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
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
* Function name ：TB6612FNG1_init
* Description   ：初始化控制第一块TB6612FNG所需的定时器，GPIO口
*                 这里设置的PWM频率为1KHz，PWM最大数值为1000
* Parameter     ：
* 		@arr
*     @psc
* Return        ：null
********************************************************************************/
static void TB6612FNG1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	// PE0:MOTOR1_IN1
	// PE1:MOTOR1_IN2
	// PE2:MOTOR2_IN1
	// PE3:MOTOR2_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_2 | GPIO_Pin_3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

	//使能GPIOD的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	// PD0:MOTOR12_STBY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// 使能GPIOB的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// PB4:MOTOR1_PWM
	// PB5:MOTOR2_PWM
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 使用复用功能输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz高速引脚
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 使用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 不使用上下拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

	// 设置为复用功能（指定具体的复用功能）
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);


	// 使能TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// 这样的设置可以让，pwm的频率为1KHz，因为时钟频率是84MHz。
	// 因为84MHz/84/1000=1KHz。另外，pwm最大只能设到1000。
	// TIM3的时钟频率是84MHz
  // 设定时基
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// 初始化输出通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	// 使能输出通道重载
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	// 使能重载寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// 使能定时器（定时器开始计数）
	TIM_Cmd(TIM3, ENABLE);

	// 使能PWM输出
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

/********************************************************************************
*
* Function name ：TB6612FNG2_init
* Description   ：初始化控制第二块TB6612FNG所需的定时器，GPIO口
*                 这里设置的PWM频率为1KHz，PWM最大数值为1000
* Parameter     ：
* 		@arr
*     @psc
* Return        ：null
********************************************************************************/
static void TB6612FNG2_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	// PA6:MOTOR3_IN1
	// PA7:MOTOR3_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
	
	//使能GPIOC的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	// PC4:MOTOR4_IN1
	// PC5:MOTOR4_IN2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5);
	
	//使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	// PE12:MOTOR34_STBY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};

	// 使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	// PE9:MOTOR3_PWM
	// PE11:MOTOR4_PWM
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_11);    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 使用复用功能输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz高速引脚
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 使用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 不使用上下拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure);    

	// 设置为复用功能（指定具体的复用功能）
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);


	// 使能TIM3时钟
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
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	// 使能输出通道重载
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	// 使能重载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	// 使能定时器（定时器开始计数）
	TIM_Cmd(TIM1, ENABLE);

	// 使能PWM输出
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/********************************************************************************
*
* Function name ：motor_startup
* Description   ：电机启动
*                 
* Parameter     ：
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_standby
* Description   ：电机准备
*                 
* Parameter     ：
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_stop
* Description   ：电机停止
*                 
* Parameter     ：
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_forward_rotate
* Description   ：电机正转
*                 
* Parameter     ：
*     @compare_value ：决定电机PWM信号的占空比
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_backward_rotate
* Description   ：电机反转
*                 
* Parameter     ：
*     @compare_value ：决定电机PWM信号的占空比
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_rotate
* Description   ：电机旋转
*                 
* Parameter     ：
*     @compare_value ：决定电机PWM信号的占空比
* 		@channel  ：电机通道，共有四路通道   
* Return        ：null
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
* Function name ：motor_init
* Description   ：电机初始化
*                 
* Parameter     ：
* 		@null      
* Return        ：null
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


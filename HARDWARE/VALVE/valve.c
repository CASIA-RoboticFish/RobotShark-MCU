/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.17
History: 在第三代boxfishstate板子上写的代码
*****************************************************************************/
#include "valve.h"

/********************************************************************************
*
* Function name ：valve_init
* Description   ：电磁阀初始化，GPIO口
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void valve_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 ); // 初始化设置为高电平（灯灭）
}

/********************************************************************************
*
* Function name ：valve_on
* Description   ：电磁阀打开
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void valve_on(uint8_t channel)
{
	switch(channel)
	{
		case VALVE1:
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			break;
		case VALVE2:
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			break;
		default:
			break;
	}
}

/********************************************************************************
*
* Function name ：valve_on
* Description   ：电磁阀关闭
*                 
* Parameter     ：
* 		@null      
* Return        ：null
********************************************************************************/
void valve_off(uint8_t channel)
{
	switch(channel)
	{
		case VALVE1:
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			break;
		case VALVE2:
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			break;
		default:
			break;
	}
}
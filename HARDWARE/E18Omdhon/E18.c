/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/

#include "E18.h"


/*****************************************************************************
函数功能： 初始化3个E18连接的I/O口，读取E18传入电平
检测到障碍——0， 没有检测到障碍——1
*****************************************************************************/
void E18_init()
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = E18_forward;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ReadInputDataBit(GPIOB, E18_forward);

}
/*****************************************************************************
函数功能： 做一个滑窗平滑，减少障碍检测跳变的0/1
平滑滤波后可采用0.5的阈值区分是否有障碍： >0.5 有障碍  <0.5 无障碍  （引入迟滞）
*****************************************************************************/
void detect_filtering()
{

}

/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/

#include "E18.h"


/*****************************************************************************
�������ܣ� ��ʼ��3��E18���ӵ�I/O�ڣ���ȡE18�����ƽ
��⵽�ϰ�����0�� û�м�⵽�ϰ�����1
*****************************************************************************/
void E18_init()
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
}

// return 1 
u8 E18_detect(u8 channel)
{
	u8 result = 0;
	switch(channel)
	{
		case E18_AHEAD:
			result = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
			break;
		
		case E18_LEFT:
			result = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
			break;
		
		case E18_RIGHT:
			result = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
			break;
		
		default:
			break;
	}
	return result;
}
/*****************************************************************************
�������ܣ� ��һ������ƽ���������ϰ���������0/1
ƽ���˲���ɲ���0.5����ֵ�����Ƿ����ϰ��� >0.5 ���ϰ�  <0.5 ���ϰ�  ��������ͣ�
*****************************************************************************/
void detect_filtering()
{

}

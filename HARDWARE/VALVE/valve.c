/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.09.17
History: �ڵ�����boxfishstate������д�Ĵ���
*****************************************************************************/
#include "valve.h"

/********************************************************************************
*
* Function name ��valve_init
* Description   ����ŷ���ʼ����GPIO��
*                 
* Parameter     ��
* 		@null      
* Return        ��null
********************************************************************************/
void valve_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOA��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 ); // ��ʼ������Ϊ�ߵ�ƽ������
}

/********************************************************************************
*
* Function name ��valve_on
* Description   ����ŷ���
*                 
* Parameter     ��
* 		@null      
* Return        ��null
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
* Function name ��valve_on
* Description   ����ŷ��ر�
*                 
* Parameter     ��
* 		@null      
* Return        ��null
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
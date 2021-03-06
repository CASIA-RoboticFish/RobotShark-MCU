/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "myiic4.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC4 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/********************************************************************************
*
* Function name ��IIC4_Init
* Description   ����ʼ��IIC4
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC4_Init(void)
{				
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB6B7��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	IIC4_SCL=1;
	IIC4_SDA=1;
}

/********************************************************************************
*
* Function name ��IIC4_Start
* Description   ������IIC4��ʼ�ź�
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC4_Start(void)
{
	IIC4_SDA_OUT();     //sda�����
	IIC4_SDA=1;	  	  
	IIC4_SCL=1;
	delay_us(4);
 	IIC4_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC4_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

/********************************************************************************
*
* Function name ��IIC4_Stop
* Description   ������IIC4ֹͣ�ź�
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC4_Stop(void)
{
	IIC4_SDA_OUT();//sda�����
	IIC4_SCL=0;
	IIC4_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC4_SCL=1; 
	IIC4_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}

/********************************************************************************
*
* Function name ��IIC4_Wait_Ack
* Description   ���ȴ�Ӧ���źŵ���
*                 
* Parameter     ��
* 		@null
* Return        ��1������Ӧ��ʧ�ܣ� 0������Ӧ��ɹ�
********************************************************************************/
u8 IIC4_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC4_SDA_IN();      //SDA����Ϊ����  
	IIC4_SDA=1;delay_us(1);	   
	IIC4_SCL=1;delay_us(1);	 
	while(IIC4_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC4_Stop();
			return 1;
		}
	}
	IIC4_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/********************************************************************************
*
* Function name ��IIC4_Ack
* Description   ������ACKӦ��
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC4_Ack(void)
{
	IIC4_SCL=0;
	IIC4_SDA_OUT();
	IIC4_SDA=0;
	delay_us(2);
	IIC4_SCL=1;
	delay_us(2);
	IIC4_SCL=0;
}

/********************************************************************************
*
* Function name ��IIC4_NAck
* Description   ��������ACKӦ��	
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/	 
void IIC4_NAck(void)
{
	IIC4_SCL=0;
	IIC4_SDA_OUT();
	IIC4_SDA=1;
	delay_us(2);
	IIC4_SCL=1;
	delay_us(2);
	IIC4_SCL=0;
}					 				     

/********************************************************************************
*
* Function name ��IIC4_Send_Byte
* Description   ��IIC4����һ���ֽ�
*                 
* Parameter     ��
* 		@null
* Return        �����شӻ�����Ӧ��1����Ӧ��0����Ӧ��		
********************************************************************************/
void IIC4_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC4_SDA_OUT(); 	    
    IIC4_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC4_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC4_SCL=1;
		delay_us(2); 
		IIC4_SCL=0;	
		delay_us(2);
    }	 
} 	    

/********************************************************************************
*
* Function name ��IIC4_Read_Byte
* Description   ����1���ֽ�
*                 
* Parameter     ��
* 		@ack      ��ack=1ʱ������ACK��ack=0������nACK��
* Return        ��null
********************************************************************************/
u8 IIC4_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC4_SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC4_SCL=0; 
		delay_us(2);
		IIC4_SCL=1;
		receive<<=1;
		if(IIC4_READ_SDA)receive++;   
		delay_us(1); 
	}					 
	if (!ack)
		IIC4_NAck();//����nACK
	else
		IIC4_Ack(); //����ACK   
	return receive;
}
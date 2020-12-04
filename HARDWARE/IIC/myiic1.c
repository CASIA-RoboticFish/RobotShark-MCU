/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "myiic1.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC1 ��������	   
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
* Function name ��IIC1_Init
* Description   ����ʼ��IIC1
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC1_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB6B7��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	IIC1_SCL=1;
	IIC1_SDA=1;
}

/********************************************************************************
*
* Function name ��IIC1_Start
* Description   ������IIC1��ʼ�ź�
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC1_Start(void)
{
	IIC1_SDA_OUT();     //sda�����
	IIC1_SDA=1;	  	  
	IIC1_SCL=1;
	delay_us(4);
 	IIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC1_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

/********************************************************************************
*
* Function name ��IIC1_Stop
* Description   ������IIC1ֹͣ�ź�
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC1_Stop(void)
{
	IIC1_SDA_OUT();//sda�����
	IIC1_SCL=0;
	IIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC1_SCL=1; 
	IIC1_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}

/********************************************************************************
*
* Function name ��IIC1_Wait_Ack
* Description   ���ȴ�Ӧ���źŵ���
*                 
* Parameter     ��
* 		@null
* Return        ��1������Ӧ��ʧ�ܣ� 0������Ӧ��ɹ�
********************************************************************************/
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC1_SDA_IN();      //SDA����Ϊ����  
	IIC1_SDA=1;delay_us(1);	   
	IIC1_SCL=1;delay_us(1);	 
	while(IIC1_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/********************************************************************************
*
* Function name ��IIC1_Ack
* Description   ������ACKӦ��
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void IIC1_Ack(void)
{
	IIC1_SCL=0;
	IIC1_SDA_OUT();
	IIC1_SDA=0;
	delay_us(2);
	IIC1_SCL=1;
	delay_us(2);
	IIC1_SCL=0;
}

/********************************************************************************
*
* Function name ��IIC1_NAck
* Description   ��������ACKӦ��	
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/	 
void IIC1_NAck(void)
{
	IIC1_SCL=0;
	IIC1_SDA_OUT();
	IIC1_SDA=1;
	delay_us(2);
	IIC1_SCL=1;
	delay_us(2);
	IIC1_SCL=0;
}					 				     

/********************************************************************************
*
* Function name ��IIC1_Send_Byte
* Description   ��IIC1����һ���ֽ�
*                 
* Parameter     ��
* 		@null
* Return        �����شӻ�����Ӧ��1����Ӧ��0����Ӧ��		
********************************************************************************/
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC1_SDA_OUT(); 	    
    IIC1_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC1_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC1_SCL=1;
		delay_us(2); 
		IIC1_SCL=0;	
		delay_us(2);
    }	 
} 	    

/********************************************************************************
*
* Function name ��IIC1_Read_Byte
* Description   ����1���ֽ�
*                 
* Parameter     ��
* 		@ack      ��ack=1ʱ������ACK��ack=0������nACK��
* Return        ��null
********************************************************************************/
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC1_SCL=0; 
		delay_us(2);
		IIC1_SCL=1;
		receive<<=1;
		if(IIC1_READ_SDA)receive++;   
		delay_us(1); 
	}					 
	if (!ack)
		IIC1_NAck();//����nACK
	else
		IIC1_Ack(); //����ACK   
	return receive;
}




























/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC1 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/********************************************************************************
*
* Function name ：IIC1_Init
* Description   ：初始化IIC1
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC1_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟


  //GPIOB6B7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	IIC1_SCL=1;
	IIC1_SDA=1;
}

/********************************************************************************
*
* Function name ：IIC1_Start
* Description   ：产生IIC1起始信号
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC1_Start(void)
{
	IIC1_SDA_OUT();     //sda线输出
	IIC1_SDA=1;	  	  
	IIC1_SCL=1;
	delay_us(4);
 	IIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC1_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

/********************************************************************************
*
* Function name ：IIC1_Stop
* Description   ：产生IIC1停止信号
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC1_Stop(void)
{
	IIC1_SDA_OUT();//sda线输出
	IIC1_SCL=0;
	IIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC1_SCL=1; 
	IIC1_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

/********************************************************************************
*
* Function name ：IIC1_Wait_Ack
* Description   ：等待应答信号到来
*                 
* Parameter     ：
* 		@null
* Return        ：1，接收应答失败； 0，接收应答成功
********************************************************************************/
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC1_SDA_IN();      //SDA设置为输入  
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
	IIC1_SCL=0;//时钟输出0 	   
	return 0;  
} 

/********************************************************************************
*
* Function name ：IIC1_Ack
* Description   ：产生ACK应答
*                 
* Parameter     ：
* 		@null
* Return        ：null
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
* Function name ：IIC1_NAck
* Description   ：不产生ACK应答	
*                 
* Parameter     ：
* 		@null
* Return        ：null
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
* Function name ：IIC1_Send_Byte
* Description   ：IIC1发送一个字节
*                 
* Parameter     ：
* 		@null
* Return        ：返回从机有无应答：1，有应答；0，无应答。		
********************************************************************************/
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC1_SDA_OUT(); 	    
    IIC1_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC1_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC1_SCL=1;
		delay_us(2); 
		IIC1_SCL=0;	
		delay_us(2);
    }	 
} 	    

/********************************************************************************
*
* Function name ：IIC1_Read_Byte
* Description   ：读1个字节
*                 
* Parameter     ：
* 		@ack      ：ack=1时，发送ACK；ack=0，发送nACK。
* Return        ：null
********************************************************************************/
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA设置为输入
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
		IIC1_NAck();//发送nACK
	else
		IIC1_Ack(); //发送ACK   
	return receive;
}




























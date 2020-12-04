/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "myiic3.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC3 驱动代码	   
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
* Function name ：IIC3_Init
* Description   ：初始化IIC3
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC3_Init(void)
{				
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟


  //GPIOB6B7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	IIC3_SCL=1;
	IIC3_SDA=1;
}

/********************************************************************************
*
* Function name ：IIC3_Start
* Description   ：产生IIC3起始信号
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC3_Start(void)
{
	IIC3_SDA_OUT();     //sda线输出
	IIC3_SDA=1;	  	  
	IIC3_SCL=1;
	delay_us(4);
 	IIC3_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC3_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

/********************************************************************************
*
* Function name ：IIC3_Stop
* Description   ：产生IIC3停止信号
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC3_Stop(void)
{
	IIC3_SDA_OUT();//sda线输出
	IIC3_SCL=0;
	IIC3_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC3_SCL=1; 
	IIC3_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

/********************************************************************************
*
* Function name ：IIC3_Wait_Ack
* Description   ：等待应答信号到来
*                 
* Parameter     ：
* 		@null
* Return        ：1，接收应答失败； 0，接收应答成功
********************************************************************************/
u8 IIC3_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC3_SDA_IN();      //SDA设置为输入  
	IIC3_SDA=1;delay_us(1);	   
	IIC3_SCL=1;delay_us(1);	 
	while(IIC3_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC3_Stop();
			return 1;
		}
	}
	IIC3_SCL=0;//时钟输出0 	   
	return 0;  
} 

/********************************************************************************
*
* Function name ：IIC3_Ack
* Description   ：产生ACK应答
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void IIC3_Ack(void)
{
	IIC3_SCL=0;
	IIC3_SDA_OUT();
	IIC3_SDA=0;
	delay_us(2);
	IIC3_SCL=1;
	delay_us(2);
	IIC3_SCL=0;
}

/********************************************************************************
*
* Function name ：IIC3_NAck
* Description   ：不产生ACK应答	
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/	 
void IIC3_NAck(void)
{
	IIC3_SCL=0;
	IIC3_SDA_OUT();
	IIC3_SDA=1;
	delay_us(2);
	IIC3_SCL=1;
	delay_us(2);
	IIC3_SCL=0;
}					 				     

/********************************************************************************
*
* Function name ：IIC3_Send_Byte
* Description   ：IIC3发送一个字节
*                 
* Parameter     ：
* 		@null
* Return        ：返回从机有无应答：1，有应答；0，无应答。		
********************************************************************************/
void IIC3_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC3_SDA_OUT(); 	    
    IIC3_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC3_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC3_SCL=1;
		delay_us(2); 
		IIC3_SCL=0;	
		delay_us(2);
    }	 
} 	    

/********************************************************************************
*
* Function name ：IIC3_Read_Byte
* Description   ：读1个字节
*                 
* Parameter     ：
* 		@ack      ：ack=1时，发送ACK；ack=0，发送nACK。
* Return        ：null
********************************************************************************/
u8 IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC3_SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		IIC3_SCL=0; 
		delay_us(2);
		IIC3_SCL=1;
		receive<<=1;
		if(IIC3_READ_SDA)receive++;   
		delay_us(1); 
	}					 
	if (!ack)
		IIC3_NAck();//发送nACK
	else
		IIC3_Ack(); //发送ACK   
	return receive;
}




























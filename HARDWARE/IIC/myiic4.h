/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MYIIC4_H
#define __MYIIC4_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC4 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO方向设置
#define IIC4_SDA_IN()  {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=0<<1*2;}	//PB9输入模式
#define IIC4_SDA_OUT() {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=1<<1*2;}  //PB9输出模式
//IO操作函数	 
#define IIC4_SCL    PAout(0) //SCL
#define IIC4_SDA    PAout(1) //SDA	 
#define IIC4_READ_SDA   PAin(1)  //输入SDA 

//IIC4所有操作函数
void IIC4_Init(void);                //初始化IIC4的IO口				 
void IIC4_Start(void);				//发送IIC4开始信号
void IIC4_Stop(void);	  			//发送IIC4停止信号
void IIC4_Send_Byte(u8 txd);			//IIC4发送一个字节
u8 IIC4_Read_Byte(unsigned char ack);//IIC4读取一个字节
u8 IIC4_Wait_Ack(void); 				//IIC4等待ACK信号
void IIC4_Ack(void);					//IIC4发送ACK信号
void IIC4_NAck(void);				//IIC4不发送ACK信号

void IIC4_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC4_Read_One_Byte(u8 daddr,u8 addr);	

#endif

















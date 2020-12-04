/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MYIIC1_H
#define __MYIIC1_H
#include "sys.h" 
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
   	   		   
//IO方向设置
#define IIC1_SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7输入模式
#define IIC1_SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7输出模式
//IO操作函数	 
#define IIC1_SCL    PBout(6) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define IIC1_READ_SDA   PBin(7)  //输入SDA 

//IIC1所有操作函数
void IIC1_Init(void);                //初始化IIC1的IO口				 
void IIC1_Start(void);				//发送IIC1开始信号
void IIC1_Stop(void);	  			//发送IIC1停止信号
void IIC1_Send_Byte(u8 txd);			//IIC1发送一个字节
u8 IIC1_Read_Byte(unsigned char ack);//IIC1读取一个字节
u8 IIC1_Wait_Ack(void); 				//IIC1等待ACK信号
void IIC1_Ack(void);					//IIC1发送ACK信号
void IIC1_NAck(void);				//IIC1不发送ACK信号

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















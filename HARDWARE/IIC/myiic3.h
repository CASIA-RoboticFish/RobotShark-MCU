/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MYIIC3_H
#define __MYIIC3_H
#include "sys.h" 
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
   	   		   
//IO方向设置
#define IIC3_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define IIC3_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}  //PB9输出模式
//IO操作函数	 
#define IIC3_SCL    PBout(8) //SCL
#define IIC3_SDA    PBout(9) //SDA	 
#define IIC3_READ_SDA   PBin(9)  //输入SDA 

//IIC3所有操作函数
void IIC3_Init(void);                //初始化IIC3的IO口				 
void IIC3_Start(void);				//发送IIC3开始信号
void IIC3_Stop(void);	  			//发送IIC3停止信号
void IIC3_Send_Byte(u8 txd);			//IIC3发送一个字节
u8 IIC3_Read_Byte(unsigned char ack);//IIC3读取一个字节
u8 IIC3_Wait_Ack(void); 				//IIC3等待ACK信号
void IIC3_Ack(void);					//IIC3发送ACK信号
void IIC3_NAck(void);				//IIC3不发送ACK信号

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	

#endif

















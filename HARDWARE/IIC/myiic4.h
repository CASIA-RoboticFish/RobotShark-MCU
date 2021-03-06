/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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
   	   		   
//IO��������
#define IIC4_SDA_IN()  {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=0<<1*2;}	//PB9����ģʽ
#define IIC4_SDA_OUT() {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=1<<1*2;}  //PB9���ģʽ
//IO��������	 
#define IIC4_SCL    PAout(0) //SCL
#define IIC4_SDA    PAout(1) //SDA	 
#define IIC4_READ_SDA   PAin(1)  //����SDA 

//IIC4���в�������
void IIC4_Init(void);                //��ʼ��IIC4��IO��				 
void IIC4_Start(void);				//����IIC4��ʼ�ź�
void IIC4_Stop(void);	  			//����IIC4ֹͣ�ź�
void IIC4_Send_Byte(u8 txd);			//IIC4����һ���ֽ�
u8 IIC4_Read_Byte(unsigned char ack);//IIC4��ȡһ���ֽ�
u8 IIC4_Wait_Ack(void); 				//IIC4�ȴ�ACK�ź�
void IIC4_Ack(void);					//IIC4����ACK�ź�
void IIC4_NAck(void);				//IIC4������ACK�ź�

void IIC4_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC4_Read_One_Byte(u8 daddr,u8 addr);	

#endif

















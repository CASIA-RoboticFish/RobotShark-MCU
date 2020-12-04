/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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
   	   		   
//IO��������
#define IIC1_SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7����ģʽ
#define IIC1_SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7���ģʽ
//IO��������	 
#define IIC1_SCL    PBout(6) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define IIC1_READ_SDA   PBin(7)  //����SDA 

//IIC1���в�������
void IIC1_Init(void);                //��ʼ��IIC1��IO��				 
void IIC1_Start(void);				//����IIC1��ʼ�ź�
void IIC1_Stop(void);	  			//����IIC1ֹͣ�ź�
void IIC1_Send_Byte(u8 txd);			//IIC1����һ���ֽ�
u8 IIC1_Read_Byte(unsigned char ack);//IIC1��ȡһ���ֽ�
u8 IIC1_Wait_Ack(void); 				//IIC1�ȴ�ACK�ź�
void IIC1_Ack(void);					//IIC1����ACK�ź�
void IIC1_NAck(void);				//IIC1������ACK�ź�

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















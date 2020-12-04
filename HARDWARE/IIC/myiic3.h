/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC3 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��������
#define IIC3_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define IIC3_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}  //PB9���ģʽ
//IO��������	 
#define IIC3_SCL    PBout(8) //SCL
#define IIC3_SDA    PBout(9) //SDA	 
#define IIC3_READ_SDA   PBin(9)  //����SDA 

//IIC3���в�������
void IIC3_Init(void);                //��ʼ��IIC3��IO��				 
void IIC3_Start(void);				//����IIC3��ʼ�ź�
void IIC3_Stop(void);	  			//����IIC3ֹͣ�ź�
void IIC3_Send_Byte(u8 txd);			//IIC3����һ���ֽ�
u8 IIC3_Read_Byte(unsigned char ack);//IIC3��ȡһ���ֽ�
u8 IIC3_Wait_Ack(void); 				//IIC3�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC3����ACK�ź�
void IIC3_NAck(void);				//IIC3������ACK�ź�

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	

#endif

















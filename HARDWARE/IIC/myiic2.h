/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MYIIC2_H
#define __MYIIC2_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��������
#define IIC2_SDA_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}	//PB10����ģʽ
#define IIC2_SDA_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;} //PB11���ģʽ
//IO��������	 
#define IIC2_SCL    PBout(10) //SCL
#define IIC2_SDA    PBout(11) //SDA	 
#define IIC2_READ_SDA   PBin(11)  //����SDA 

//IIC2���в�������
void IIC2_Init(void);                //��ʼ��IIC2��IO��				 
void IIC2_Start(void);				//����IIC2��ʼ�ź�
void IIC2_Stop(void);	  			//����IIC2ֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC2����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC2�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC2����ACK�ź�
void IIC2_NAck(void);				//IIC2������ACK�ź�

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















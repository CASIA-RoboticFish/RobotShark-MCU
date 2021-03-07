/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.12.24
History: 
*****************************************************************************/

#ifndef __TFMINI_H
#define __TFMINI_H

#include "myiic2.h"
#include "delay.h"
#include "sys.h" 

#define TFMINI_DATA_LEN             9                   // TFMINI ���ݷ���֡��
#define TFMINI_DATA_HEAD            0x59                // TFMINI ����֡ ͷ֡�ʹ�֡���� 0x59    
#define TFMINI_ACTION_DIST          200                 // ���̴���ģ�1/0��ģʽ�ķֽ���ֵ
#define TFMINI_ADDRESS              0x10                // ��ʼ����TFMINI�豸��ַ ����˵��������޸�


// ��ʼ��TFmini ģ�� ����ʼ��IIC_2��
void TFmini_init();
// IIC_2 ��ȡTFmini�ķ���ֵ  ���ݳ���Ϊ9
u8 TFmini_read_bytes(u8 dev, u8 read_length, uint8_t send_length, uint8_t *send_data, uint8_t *read_data);
// IIC_2  д���� �����������Զ����豸�Ĳ�����
u8 TFmini_write_bytes(u8 dev, u8 length, u8 *data);
// TFMINI����֡���� ����[0]: ����ֵ [1]: ǿ��ֵ���Ĵ���results�����ֵ��1/0��Ϊ������Ϣ����ȷ�ԡ�
uint8_t TFmini_data_analyse(uint8_t *data, uint16_t *results);
// ��ȡTFmini����
uint16_t TFmini_get_distance();
#endif
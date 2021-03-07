/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
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

#define TFMINI_DATA_LEN             9                   // TFMINI 数据返回帧长
#define TFMINI_DATA_HEAD            0x59                // TFMINI 数据帧 头帧和次帧数据 0x59    
#define TFMINI_ACTION_DIST          200                 // 例程代码的（1/0）模式的分界阈值
#define TFMINI_ADDRESS              0x10                // 初始化的TFMINI设备地址 查阅说明书可以修改


// 初始化TFmini 模块 （初始化IIC_2）
void TFmini_init();
// IIC_2 读取TFmini的返回值  数据长度为9
u8 TFmini_read_bytes(u8 dev, u8 read_length, uint8_t send_length, uint8_t *send_data, uint8_t *read_data);
// IIC_2  写数据 （可以用来自定义设备的参数）
u8 TFmini_write_bytes(u8 dev, u8 length, u8 *data);
// TFMINI数据帧解析 保存[0]: 距离值 [1]: 强度值到寄存器results里，返回值（1/0）为接受信息的正确性。
uint8_t TFmini_data_analyse(uint8_t *data, uint16_t *results);
// 读取TFmini数据
uint16_t TFmini_get_distance();
#endif
/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2020.12.24
History: 
*****************************************************************************/
#ifndef __JY901_H_
#define JY901___JY901_H_
#include "myiic2.h"
#include "delay.h"

#define JY901_SAVE 		0x00
#define JY901_CALSW 		0x01
#define JY901_RSW 		0x02
#define JY901_RRATE		0x03
#define JY901_BAUD 		0x04
#define JY901_AXOFFSET	        0x05
#define JY901_AYOFFSET		0x06
#define JY901_AZOFFSET		0x07
#define JY901_GXOFFSET		0x08
#define JY901_GYOFFSET		0x09
#define JY901_GZOFFSET		0x0a
#define JY901_HXOFFSET		0x0b
#define JY901_HYOFFSET		0x0c
#define JY901_HZOFFSET		0x0d
#define JY901_D0MODE		0x0e
#define JY901_D1MODE		0x0f
#define JY901_D2MODE		0x10
#define JY901_D3MODE		0x11
#define JY901_D0PWMH		0x12
#define JY901_D1PWMH		0x13
#define JY901_D2PWMH		0x14
#define JY901_D3PWMH		0x15
#define JY901_D0PWMT		0x16
#define JY901_D1PWMT		0x17
#define JY901_D2PWMT		0x18
#define JY901_D3PWMT		0x19
#define JY901_IICADDR		0x1a
#define JY901_LEDOFF 		0x1b
#define JY901_GPSBAUD		0x1c

#define JY901_YYMM		0x30
#define JY901_DDHH		0x31
#define JY901_MMSS		0x32
#define JY901_MS		0x33
#define JY901_AX		0x34
#define JY901_AY		0x35
#define JY901_AZ		0x36
#define JY901_GX		0x37
#define JY901_GY		0x38
#define JY901_GZ		0x39
#define JY901_HX		0x3a
#define JY901_HY		0x3b
#define JY901_HZ		0x3c			
#define JY901_Roll		0x3d
#define JY901_Pitch		0x3e
#define JY901_Yaw		0x3f
#define JY901_TEMP		0x40
#define JY901_D0Status		0x41
#define JY901_D1Status		0x42
#define JY901_D2Status		0x43
#define JY901_D3Status		0x44
#define JY901_PressureL		0x45
#define JY901_PressureH		0x46
#define JY901_HeightL		0x47
#define JY901_HeightH		0x48
#define JY901_LonL		0x49
#define JY901_LonH		0x4a
#define JY901_LatL		0x4b
#define JY901_LatH		0x4c
#define JY901_GPSHeight         0x4d
#define JY901_GPSYAW          	0x4e
#define JY901_GPSVL		0x4f
#define JY901_GPSVH		0x50
      
#define JY901_DIO_MODE_AIN 0
#define JY901_DIO_MODE_DIN 1
#define JY901_DIO_MODE_DOH 2
#define JY901_DIO_MODE_DOL 3
#define JY901_DIO_MODE_DOPWM 4
#define JY901_DIO_MODE_GPS 5			

void JY901_init();
u8 JY901_write_bytes(u8 dev, u8 reg, u8 length, u8* data);
u8 JY901_read_bytes(u8 dev, u8 reg, u8 length, u8 *data);
#endif
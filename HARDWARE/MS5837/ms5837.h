/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __MS5837_H
#define __MS5837_H
#include "sys.h" 
#include "delay.h"

#define MS583703BA_ADC_RD          0x00
#define	MS583703BA_PROM_RD 	       0xA0
#define MS583703BA_PROM_CRC        0xAE

#define MS583703BA_SlaveAddress    0xEE  //MS5387的地址
#define MS583703BA_RST             0x1E  //cmd 复位

#define	MS583703BA_D2_OSR_4096   	 0x5A	// 9.04 mSec conversion time ( 110.62 Hz)
#define	MS583703BA_D1_OSR_4096   	 0x4A

#define MS5837_OSR256					 		 0x40
#define MS5837_OSR512					 		 0x42
#define MS5837_OSR1024					   0x44
#define MS5837_OSR2048					   0x46
#define MS5837_OSR4096					   0x48
#define MS5837_OSR8192					   0x58


double MS583703BA_getPressure(void);
void MS583703BA_getTemperature(void);
unsigned long MS583703BA_getConversion(uint8_t command);

void ms5837_init(void);
void ms5837_reset(void);
float get_ms5837_data(void);


#endif
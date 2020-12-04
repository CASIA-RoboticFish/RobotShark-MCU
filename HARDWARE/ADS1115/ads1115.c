/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "ads1115.h"
#include "myiic1.h"
#include "BuffPrintf.h"


static void writeRegister(uint8_t reg, uint16_t value) 
{
	IIC1_Start();
	IIC1_Send_Byte(ADS1115_WRITE_ADDRESS);
	if(IIC1_Wait_Ack())
		return;
	IIC1_Send_Byte((uint8_t)reg);
	if(IIC1_Wait_Ack())
		return;
	IIC1_Send_Byte((uint8_t)(value>>8));
	if(IIC1_Wait_Ack())
		return;
	IIC1_Send_Byte((uint8_t)(value & 0xFF));
	if(IIC1_Wait_Ack())
		return;
	IIC1_Stop();
}



static uint16_t readRegister(uint8_t reg) 
{
  IIC1_Start();
	IIC1_Send_Byte(ADS1115_WRITE_ADDRESS);
	delay_us(20);
	if(IIC1_Wait_Ack())
		return 0;
	IIC1_Send_Byte(ADS1115_REG_POINTER_CONVERT);
	if(IIC1_Wait_Ack())
		return 0;
	IIC1_Stop();
	
	uint16_t data;
	uint8_t read_buffer[2];
	IIC1_Start();
	IIC1_Send_Byte(ADS1115_READ_ADDRESS);
	if(IIC1_Wait_Ack())
		return 0;
	read_buffer[0] = IIC1_Read_Byte(1);
	read_buffer[1] = IIC1_Read_Byte(1);
	IIC1_Stop();
	data = read_buffer[0]*256 + read_buffer[1];
	return data;
}



uint16_t read_adc_singleended(uint8_t channel)
{
	if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  uint16_t config = ADS1115_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1115_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1115_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1115_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1115_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= GAIN_ONE;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1115_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(ADS1115_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  delay_ms(ADS1115_CONVERSIONDELAY);

  // Read the conversion results
  return readRegister(ADS1115_REG_POINTER_CONVERT);  
}

void ads1115_init()
{
	IIC1_Init();
}

uint16_t get_ads1115_data(uint8_t channel)
{
	return read_adc_singleended(channel);
}
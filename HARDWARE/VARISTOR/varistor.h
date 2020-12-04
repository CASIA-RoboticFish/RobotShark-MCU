#ifndef __VARISTOR_H_
#define __VARISTOR_H_

#include "stm32f4xx.h"
#define VARISTOR1 4
#define VARISTOR2 5
void  varistor_adc_init(void);
u16 get_adc(u8 ch);
u16 get_adc_average(u8 ch,u8 times);
u16 get_varistor_value(uint8_t channel);

#endif
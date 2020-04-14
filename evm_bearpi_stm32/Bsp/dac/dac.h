#ifndef __DAC_H
#define __DAC_H
#include "stm32l4xx_hal.h"

void DAC1_Init(void);
void DAC1_DeInit(void);
void DAC1_Set_Value(uint16_t value);
#endif

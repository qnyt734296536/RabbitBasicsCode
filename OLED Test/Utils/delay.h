#ifndef __DELAY_H_
#define __DELAY_H_ 			   

#include "stm32f1xx_hal.h"
 
void Delay_Init(uint8_t sysclk);
void Delay_Ms(uint16_t nms);
void Delay_Us(uint32_t nus);

#endif






























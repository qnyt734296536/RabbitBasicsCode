#ifndef _HAL_INF_RECEIVER_H_
#define _HAL_INF_RECEIVER_H_


#include "common.h"


#define INF_RECEIVER_PIN GPIO_PIN_2
#define INF_RECEIVER_GPIO_TYPE GPIOD


void Pro_Inf_Receiver_Pin_Eint(void);

void Test_Inf_Receiver(void);

#endif


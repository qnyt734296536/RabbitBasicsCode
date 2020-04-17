#include "Hal_Pyr.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char state = 0;
	if (GPIO_Pin == PYR_PIN)
	{
		state = HAL_GPIO_ReadPin(PYR_GPIO_TYPE, PYR_PIN);
		LOG("pyr state is %d\r\n",state);
	}
}

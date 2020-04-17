#include "Hal_Infrared_Reflective.h"

//GPIO的中断回调函数，需要自己实现
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char state = 0;
	if(GPIO_Pin == GPIO_PIN_1)
	{
		state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
		LOG("inf state=%d\r\n",state);
	}

}




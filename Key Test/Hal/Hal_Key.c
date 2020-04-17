#include "Hal_Key.h"


//GPIO的中断回调函数，需要自己实现
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char state = 0;
	if(GPIO_Pin == KEY1_PIN)
	{
		state = HAL_GPIO_ReadPin(KEY1_GPIO_TYPE, KEY1_PIN);
		LOG("key1=%d\r\n",state);
	}
	
	if(GPIO_Pin == KEY2_PIN)
	{
		state = HAL_GPIO_ReadPin(KEY2_GPIO_TYPE, KEY2_PIN);
		LOG("key2=%d\r\n",state);
	}
	
	if(GPIO_Pin == KEY3_PIN)
	{
		state = HAL_GPIO_ReadPin(KEY3_GPIO_TYPE, KEY3_PIN);
		LOG("key3=%d\r\n",state);
	}

}





#include "Hal_Led.h"
#include "stm32f1xx_hal.h"
#include "delay.h"
/**
*设置所有LED为开状态
*/
void Set_Led_On(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
}


/**
*设置所有LED为关状态
*/
void Set_Led_Off(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
}


void Led_Test(void)
{
	static int count =0;
	if(count == 1)
	{
		Set_Led_On();
	}
	if(count == 1000)
	{
		Set_Led_Off();
	}
	
	if(count > 2000)
	{
		count =0;
	}
	count ++;
	
}

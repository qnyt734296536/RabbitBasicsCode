#include "Hal_Buzzer.h"

void Test_Buzzer(void)
{
	static int count=0;
	if(count== 1)
	{
		LOG("buzzer!\r\n");
		HAL_GPIO_WritePin(BUZZER_GPIO_TYPE,BUZZER_PIN,GPIO_PIN_SET);
		Delay_Ms(50); //调节延迟能得到不同的声音
		HAL_GPIO_WritePin(BUZZER_GPIO_TYPE,BUZZER_PIN,GPIO_PIN_RESET);
		Delay_Ms(100);
	}
	count++; //count 延时，是非阻塞的方式延迟
	if(count > 100) 
	{
		count=0;
	}
}

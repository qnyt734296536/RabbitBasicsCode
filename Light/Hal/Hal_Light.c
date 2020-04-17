#include "Hal_Light.h"

extern ADC_HandleTypeDef hadc1;

/**
*获取光照强度等级
*/
uint8_t Get_Light_Str_Garde(void)
{
	uint16_t value =0;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);    //等待转换完成，第二个参数表示超时时间，单位ms        
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	{
	     value=HAL_ADC_GetValue(&hadc1);
	}                   
	LOG("PC0 ADC : %.4f \r\n",((float)value*3.3)/4096);        
	return 0;
}



void Light_Test(void)
{
	static int count = 0;
	
	if(count == 1)
	{
		Get_Light_Str_Garde();
	}

	if(count > 2000)
	{
		count=0;
	}
	count++;
}


